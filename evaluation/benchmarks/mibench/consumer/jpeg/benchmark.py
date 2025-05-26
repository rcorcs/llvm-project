import glob
from pathlib import Path
from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

cjpeg = Benchmark(
    name='cjpeg',
    path=DIR,
    src_files=[DIR / 'jpeg-6a' / f for f in
               ['cjpeg.c', 'rdppm.c', 'rdgif.c', 'rdtarga.c', 'rdrle.c', 'rdbmp.c', 'rdswitch.c', 'cdjpeg.c',
                'jcapimin.c', 'jcapistd.c', 'jctrans.c', 'jcparam.c', 'jdatadst.c', 'jcinit.c', 'jcmaster.c',
                'jcmarker.c', 'jcmainct.c', 'jcprepct.c', 'jccoefct.c', 'jccolor.c', 'jcsample.c', 'jchuff.c',
                'jcphuff.c', 'jcdctmgr.c', 'jfdctfst.c', 'jfdctflt.c', 'jfdctint.c', 'jdapimin.c', 'jdapistd.c',
                'jdtrans.c', 'jdatasrc.c', 'jdmaster.c', 'jdinput.c', 'jdmarker.c', 'jdhuff.c', 'jdphuff.c',
                'jdmainct.c', 'jdcoefct.c', 'jdpostct.c', 'jddctmgr.c', 'jidctfst.c', 'jidctflt.c', 'jidctint.c',
                'jidctred.c', 'jdsample.c', 'jdcolor.c', 'jquant1.c', 'jquant2.c', 'jdmerge.c', 'jcomapi.c', 'jutils.c',
                'jerror.c', 'jmemmgr.c', 'jmemnobs.c']],
    build_flags=[],
    workloads=[
        Workload('ppm-to-jpeg', ['-dct', 'int', '-progressive', '-opt', '-outfile', DIR / 'output_large_encode.jpeg', DIR / 'input_large.ppm'], None, None),
    ]
)

djpeg = Benchmark(
    name='djpeg',
    path=DIR,
    src_files=[DIR / 'jpeg-6a' / f for f in
               ['djpeg.c', 'wrppm.c', 'wrgif.c', 'wrtarga.c', 'wrrle.c', 'wrbmp.c', 'rdcolmap.c', 'cdjpeg.c',
                'jcapimin.c', 'jcapistd.c', 'jctrans.c', 'jcparam.c', 'jdatadst.c', 'jcinit.c', 'jcmaster.c',
                'jcmarker.c', 'jcmainct.c', 'jcprepct.c', 'jccoefct.c', 'jccolor.c', 'jcsample.c', 'jchuff.c',
                'jcphuff.c', 'jcdctmgr.c', 'jfdctfst.c', 'jfdctflt.c', 'jfdctint.c', 'jdapimin.c', 'jdapistd.c',
                'jdtrans.c', 'jdatasrc.c', 'jdmaster.c', 'jdinput.c', 'jdmarker.c', 'jdhuff.c', 'jdphuff.c',
                'jdmainct.c', 'jdcoefct.c', 'jdpostct.c', 'jddctmgr.c', 'jidctfst.c', 'jidctflt.c', 'jidctint.c',
                'jidctred.c', 'jdsample.c', 'jdcolor.c', 'jquant1.c', 'jquant2.c', 'jdmerge.c', 'jcomapi.c', 'jutils.c',
                'jerror.c', 'jmemmgr.c', 'jmemnobs.c']],
    build_flags=[],
    workloads=[
        Workload('jpeg-to-ppm',
                 ['-dct', 'int', '-ppm', '-outfile', DIR / 'output_large_decode.ppm', DIR / 'input_large.jpg'], None,
                 None),
    ]
)
