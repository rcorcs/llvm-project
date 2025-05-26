from pathlib import Path
from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

benchmark = Benchmark(
    name='qsort',
    path=DIR,
    src_files=[DIR / f for f in ['qsort_large.c']],
    build_flags=[],
    workloads=[Workload(None, [DIR / 'input_large.dat'], None, DIR / 'output_large.txt')],
)
