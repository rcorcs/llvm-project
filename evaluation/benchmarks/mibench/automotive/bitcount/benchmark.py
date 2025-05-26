from pathlib import Path
from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

benchmark = Benchmark(
    name='bitcount',
    path=DIR,
    src_files=[DIR / f for f in
               ['bitcnt_1.c', 'bitcnt_2.c', 'bitcnt_3.c', 'bitcnt_4.c', 'bitcnts.c', 'bitfiles.c', 'bitstrng.c',
                'bstr_i.c']],
    build_flags=[],
    workloads=[Workload(None, ['1125000'], None, DIR / 'output_large.txt')]
)
