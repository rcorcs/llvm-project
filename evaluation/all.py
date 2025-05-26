import os
from pathlib import Path
from llvm import LLVM

# SQLite
from benchmarks.sqlite.benchmark import benchmark as sqlite
# MiBench
from benchmarks.mibench.automotive.basicmath.benchmark import benchmark as basicmath
from benchmarks.mibench.automotive.bitcount.benchmark import benchmark as bitcount
from benchmarks.mibench.automotive.qsort.benchmark import benchmark as qsort
from benchmarks.mibench.automotive.susan.benchmark import benchmark as susan
from benchmarks.mibench.consumer.jpeg.benchmark import cjpeg as cjpeg
from benchmarks.mibench.consumer.jpeg.benchmark import djpeg as djpeg
from benchmarks.mibench.consumer.lame.benchmark import benchmark as lame

LLVM_DIR = Path(os.environ['LLVM_DIR'])


def main():
    llvm = LLVM(LLVM_DIR)

    benchmarks = [
        sqlite,

        basicmath,
        bitcount,
        qsort,
        susan,
        cjpeg,
        djpeg,
        lame,
    ]

    sizes = []
    for benchmark in benchmarks:
        print(f'[benchmark] {benchmark.name}')

        print(f'[emit-ir] {benchmark.ir}')
        benchmark.emit_ir(llvm)

        # print(f'[run]')
        # benchmark.run(llvm)

        def run_opt_then_size(benchmark, flags):
            print(f'[optimize] {flags}')
            opt_ir = benchmark.ir.with_suffix('.opt.ll')
            llvm.optimize(benchmark.ir, opt_ir, flags)
            print(f'[codegen]')
            obj = opt_ir.with_suffix('.o')
            llvm.codegen(opt_ir, obj)
            return llvm.code_size(obj)

        constant_args_flags = ['-passes=func-cloning', '-fe-constants-only=true']
        before_size = run_opt_then_size(benchmark, constant_args_flags)

        cloning_flags = ['-passes=func-cloning', '-fe-constants-only=false']
        after_size = run_opt_then_size(benchmark, cloning_flags)

        print(f'[objsize] {before_size} --> {after_size}')

        sizes.append((before_size, after_size))

    # print like a table
    print(f'{"Benchmark":>15} {"constargs":>10} {"cloning":>10} {"diff(Bytes)":>12} {"diff(%)":>8}')
    for benchmark, (before_size, after_size) in zip(benchmarks, sizes):
        diff = -(after_size - before_size)
        print(f'{benchmark.name:>15} {before_size:>10} {after_size:>10} {diff :>12} {100 * diff / before_size:>8.2f}')


if __name__ == '__main__':
    main()
