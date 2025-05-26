from pathlib import Path

from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

benchmark = Benchmark(
    name='basicmath',
    path=DIR,
    src_files=[DIR / f for f in ['basicmath_large.c', 'rad2deg.c', 'cubic.c', 'isqrt.c']],
    build_flags=[],
    workloads=[Workload(None, [], None, DIR / 'output_large.txt')]
)
