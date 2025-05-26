from pathlib import Path
from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

benchmark = Benchmark(
    name='susan',
    path=DIR,
    src_files=[DIR / f for f in ['susan.c']],
    build_flags=[],
    workloads=[
        Workload('smoothing', [DIR / 'input_large.pgm', DIR / 'output_large.smoothing.pgm', '-s'], None, None),
        Workload('edges', [DIR / 'input_large.pgm', DIR / 'output_large.edges.pgm', '-e'], None, None),
        Workload('corners', [DIR / 'input_large.pgm', DIR / 'output_large.corners.pgm', '-c'], None, None),
    ]
)
