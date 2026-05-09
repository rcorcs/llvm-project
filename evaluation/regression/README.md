# Loop/Region Rolling Regression Examples

This directory owns the small `.c` examples used by the repeatable regression
suite for both rolling modes:

- `looprolling`: `opt -loop-rolling -loop-rolling-extensions=false`
- `regionrolling`: `opt -loop-rolling -loop-rolling-extensions=true`

Run from this directory:

```bash
python3 run_regressions.py
```

The runner writes:

- `out/results.csv`: one row per case/pass
- `out/summary.txt`: category counts
- `out/<case>/...`: IR, object files, and logs for each case

Categories are:

- `improved`: object text size decreased
- `regressed`: object text size increased
- `same_size_changed`: IR changed, but object text size did not
- `conservative_bailout`: pass succeeded and did not reduce size, with logs indicating an intentional skip such as unprofitable, invalid generated code, or no candidate found
- `unchanged`: pass succeeded without changing size or IR
- `compile_fail`, `pass_fail`, `codegen_fail`: hard failures

To snapshot the current behavior as expectations:

```bash
python3 run_regressions.py --update-expectations expectations.csv
```

Future runs compare against `expectations.csv` by default and mark rows whose
category changed.

Input layout:

- `cases/root/`: small standalone examples.
- `cases/phi/`: PHI and region/PHI examples.
- `cases/reduction/`: reduction examples.
- `cases/runnables/`: runnable region examples kept as compiler regressions.
- `angha/`: reduced AnghaBench reproducers for fixed RegionRolling bugs.

The larger historical TSVC aggregate remains in `../TSVC/`; disabled entries in
`cases.csv` can still point there when those broader experiments are needed.
