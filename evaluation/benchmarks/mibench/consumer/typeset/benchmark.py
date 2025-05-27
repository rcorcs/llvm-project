from pathlib import Path

from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

benchmark = Benchmark(
    name='typeset',
    path=DIR,
    src_files=[DIR / 'lout-3.24' / f for f in
               [
                   'z01.c', 'z02.c', 'z03.c', 'z04.c', 'z05.c', 'z06.c', 'z07.c', 'z08.c', 'z09.c', 'z10.c', 'z11.c',
                   'z12.c', 'z13.c', 'z14.c', 'z15.c', 'z16.c', 'z17.c', 'z18.c', 'z19.c', 'z20.c', 'z21.c', 'z22.c',
                   'z23.c', 'z24.c', 'z25.c', 'z26.c', 'z27.c', 'z28.c', 'z29.c', 'z30.c', 'z31.c', 'z32.c', 'z33.c',
                   'z34.c', 'z35.c', 'z36.c', 'z37.c', 'z38.c', 'z39.c', 'z40.c', 'z41.c', 'z42.c', 'z43.c', 'z44.c',
                   'z45.c', 'z46.c', 'z47.c', 'z48.c', 'z49.c', 'z50.c', 'z51.c']
               ],

    build_flags=[
        '-DOS_UNIX=1',
        '-DOS_DOS=0',
        '-DOS_MAC=0',
        '-DDB_FIX=0',
        '-DUSE_STAT=1',
        '-DSAFE_DFT=0',
        '-DCOLLATE=1',
        '-DLIB_DIR="/usr/staff/jeff/lout.lib"',
        '-DFONT_DIR="font"',
        '-DMAPS_DIR="maps"',
        '-DINCL_DIR="include"',
        '-DDATA_DIR="data"',
        '-DHYPH_DIR="hyph"',
        '-DLOCALE_DIR="locale"',
        '-DCHAR_IN=1',
        '-DCHAR_OUT=0',
        '-DLOCALE_ON=1',
        '-DASSERT_ON=1',
        '-ansi',
        '-pedantic',
        '-Wall',
        '-O3',
        '-DDEBUG_ON=0',
        '-DPDF_COMPRESSION=0'
    ],
    workloads=[
        Workload(None,
                 [
                     f'-I{DIR / "lout-3.24" / "include"}',
                     f'-D{DIR / "lout-3.24" / "data"}',
                     f'-F{DIR / "lout-3.24" / "font"}',
                     f'-C{DIR / "lout-3.24" / "maps"}',
                     f'-H{DIR / "lout-3.24" / "hyph"}',
                     DIR / 'large.lout'
                 ],
                 None,
                 DIR / 'output_large.ps')
    ]
)
