#!/usr/bin/env python3
import argparse
import csv
import difflib
import hashlib
import os
from pathlib import Path
import shutil
import subprocess
import sys


PASSES = {
    "looprolling": ["-loop-rolling", "-loop-rolling-extensions=false"],
    "regionrolling": ["-loop-rolling", "-loop-rolling-extensions=true"],
}

BAILOUT_PATTERNS = (
    "Invalid generated region code",
    "Region Unprofitable",
    "Unprofitable",
    "Nothing found",
    "Skipping wide aligned graph",
    "could not map to entry block",
    "ERROR: Null InAB",
)


def run(cmd, stdout_path, stderr_path, timeout):
    with stdout_path.open("wb") as out, stderr_path.open("wb") as err:
        try:
            proc = subprocess.run(cmd, stdout=out, stderr=err, timeout=timeout)
            return proc.returncode
        except subprocess.TimeoutExpired:
            err.write(f"\nTIMEOUT after {timeout}s\n".encode())
            return 124


def read_text(path):
    try:
        return path.read_text(errors="replace")
    except FileNotFoundError:
        return ""


def file_sha1(path):
    h = hashlib.sha1()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def normalized_ir_changed(before, after):
    if not before.exists() or not after.exists():
        return ""
    before_lines = [
        line
        for line in read_text(before).splitlines()
        if not line.startswith("; ModuleID =") and not line.startswith("source_filename =")
    ]
    after_lines = [
        line
        for line in read_text(after).splitlines()
        if not line.startswith("; ModuleID =") and not line.startswith("source_filename =")
    ]
    return "1" if before_lines != after_lines else "0"


def text_size(llvm_size, obj_path):
    out = subprocess.check_output([str(llvm_size), "-format=sysv", str(obj_path)], text=True)
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 2 and parts[0] == "__text":
            return int(parts[1])
    return None


def classify(compile_status, pass_status, codegen_status, before_text, after_text, ir_changed, log_text):
    if compile_status != "ok":
        return "compile_fail"
    if pass_status != "ok":
        return "pass_fail"
    if codegen_status != "ok":
        return "codegen_fail"
    if before_text is None or after_text is None:
        return "size_unknown"
    if after_text < before_text:
        return "improved"
    if after_text > before_text:
        return "regressed"
    if any(pattern in log_text for pattern in BAILOUT_PATTERNS):
        return "conservative_bailout"
    if ir_changed == "1":
        return "same_size_changed"
    return "unchanged"


def load_cases(path):
    cases = []
    with path.open(newline="") as f:
        for row in csv.DictReader(f):
            if row.get("enabled", "1") not in ("1", "true", "True", "yes"):
                continue
            cases.append(row)
    return cases


def load_expectations(path):
    expectations = {}
    if not path.exists():
        return expectations
    with path.open(newline="") as f:
        for row in csv.DictReader(f):
            expectations[(row["case"], row["pass"])] = row["category"]
    return expectations


def write_expectations(path, rows):
    with path.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=["case", "path", "pass", "category", "text_before", "text_after"])
        writer.writeheader()
        for row in rows:
            writer.writerow({
                "case": row["case"],
                "path": row["path"],
                "pass": row["pass"],
                "category": row["category"],
                "text_before": row["text_before"],
                "text_after": row["text_after"],
            })


def run_case(case, args, llvm_bin, out_dir, expectations):
    src = (args.cases.parent / case["path"]).resolve()
    case_dir = out_dir / case["case"]
    case_dir.mkdir(parents=True, exist_ok=True)

    base_ll = case_dir / "baseline.ll"
    base_obj = case_dir / "baseline.o"
    compile_out = case_dir / "compile.stdout"
    compile_err = case_dir / "compile.stderr"

    clang = llvm_bin / "clang"
    opt = llvm_bin / "opt"
    llvm_size = llvm_bin / "llvm-size"

    compile_cmd = [
        str(clang),
        str(src),
        args.opt_level,
        "-w",
        "-emit-llvm",
        "-S",
        "-o",
        str(base_ll),
        "-fno-vectorize",
        "-fno-slp-vectorize",
        "-fno-unroll-loops",
    ]
    if args.archopt:
        compile_cmd.extend(args.archopt.split())

    rc = run(compile_cmd, compile_out, compile_err, args.timeout)
    compile_status = "ok" if rc == 0 and base_ll.exists() else f"fail:{rc}"

    base_text = None
    codegen_base_status = "skip"
    if compile_status == "ok":
        base_obj_out = case_dir / "baseline-obj.stdout"
        base_obj_err = case_dir / "baseline-obj.stderr"
        rc = run(
            [str(clang), str(base_ll), args.opt_level, "-c", "-o", str(base_obj)],
            base_obj_out,
            base_obj_err,
            args.timeout,
        )
        codegen_base_status = "ok" if rc == 0 and base_obj.exists() else f"fail:{rc}"
        if codegen_base_status == "ok":
            base_text = text_size(llvm_size, base_obj)

    rows = []
    for pass_name, pass_flags in PASSES.items():
        pass_ll = case_dir / f"{pass_name}.ll"
        pass_obj = case_dir / f"{pass_name}.o"
        pass_out = case_dir / f"{pass_name}.stdout"
        pass_err = case_dir / f"{pass_name}.stderr"
        obj_out = case_dir / f"{pass_name}-obj.stdout"
        obj_err = case_dir / f"{pass_name}-obj.stderr"

        pass_status = "skip"
        codegen_status = "skip"
        after_text = None
        ir_changed = ""

        if compile_status == "ok" and codegen_base_status == "ok":
            rc = run([str(opt), *pass_flags, str(base_ll), "-o", str(pass_ll), "-S"], pass_out, pass_err, args.timeout)
            pass_status = "ok" if rc == 0 and pass_ll.exists() else f"fail:{rc}"

        if pass_status == "ok":
            rc = run([str(clang), str(pass_ll), args.opt_level, "-c", "-o", str(pass_obj)], obj_out, obj_err, args.timeout)
            codegen_status = "ok" if rc == 0 and pass_obj.exists() else f"fail:{rc}"
            if codegen_status == "ok":
                after_text = text_size(llvm_size, pass_obj)
                ir_changed = normalized_ir_changed(base_ll, pass_ll)

        log_text = read_text(pass_err) + read_text(pass_out)
        category = classify(compile_status, pass_status, codegen_status, base_text, after_text, ir_changed, log_text)
        expected = expectations.get((case["case"], pass_name), "")
        rows.append({
            "case": case["case"],
            "path": case["path"],
            "pass": pass_name,
            "compile_status": compile_status,
            "baseline_codegen_status": codegen_base_status,
            "pass_status": pass_status,
            "codegen_status": codegen_status,
            "text_before": "" if base_text is None else str(base_text),
            "text_after": "" if after_text is None else str(after_text),
            "text_delta": "" if base_text is None or after_text is None else str(after_text - base_text),
            "ir_changed": ir_changed,
            "category": category,
            "expected_category": expected,
            "expectation_changed": "1" if expected and expected != category else "0",
            "case_dir": str(case_dir),
        })

    if args.copy_sources:
        shutil.copy2(src, case_dir / src.name)

    return rows


def main():
    script_dir = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description="Run LoopRolling and RegionRolling regression examples.")
    parser.add_argument("--cases", type=Path, default=script_dir / "cases.csv")
    parser.add_argument("--llvm-bin", type=Path, default=script_dir / "../../build/release/bin")
    parser.add_argument("--out-dir", type=Path, default=script_dir / "out")
    parser.add_argument("--expectations", type=Path, default=script_dir / "expectations.csv")
    parser.add_argument("--update-expectations", type=Path)
    parser.add_argument("--opt-level", default="-Os", choices=["-O0", "-O1", "-O2", "-O3", "-Os", "-Oz"])
    parser.add_argument("--archopt", default=os.environ.get("ARCHOPT", ""))
    parser.add_argument("--timeout", type=int, default=30)
    parser.add_argument("--copy-sources", action="store_true")
    args = parser.parse_args()

    llvm_bin = args.llvm_bin.resolve()
    out_dir = args.out_dir.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    cases = load_cases(args.cases)
    expectations = load_expectations(args.expectations)
    all_rows = []
    for case in cases:
        print(f"running {case['case']}")
        all_rows.extend(run_case(case, args, llvm_bin, out_dir, expectations))

    fieldnames = [
        "case",
        "path",
        "pass",
        "compile_status",
        "baseline_codegen_status",
        "pass_status",
        "codegen_status",
        "text_before",
        "text_after",
        "text_delta",
        "ir_changed",
        "category",
        "expected_category",
        "expectation_changed",
        "case_dir",
    ]
    results_csv = out_dir / "results.csv"
    with results_csv.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(all_rows)

    counts = {}
    changed = 0
    for row in all_rows:
        counts[(row["pass"], row["category"])] = counts.get((row["pass"], row["category"]), 0) + 1
        changed += row["expectation_changed"] == "1"

    summary = out_dir / "summary.txt"
    with summary.open("w") as f:
        for key in sorted(counts):
            f.write(f"{key[0]},{key[1]},{counts[key]}\n")
        f.write(f"expectation_changed,{changed}\n")

    if args.update_expectations:
        write_expectations(args.update_expectations, all_rows)

    print(f"wrote {results_csv}")
    print(f"wrote {summary}")
    if changed:
        print(f"ERROR: {changed} expectation(s) changed", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
