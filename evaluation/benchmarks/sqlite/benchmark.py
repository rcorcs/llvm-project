import os
from pathlib import Path

from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

benchmark = Benchmark(
    name='sqlite',
    path=DIR,
    src_files=[DIR / 'src' / f for f in ['sqlite3.c', 'shell.c']],
    build_flags=[],
    workloads=[
        Workload(None, [], DIR / 'workloads' / '1.sql', None),
    ]
)
