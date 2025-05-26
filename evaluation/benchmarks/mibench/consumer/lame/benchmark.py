from pathlib import Path

from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

benchmark = Benchmark(
    name='lame',
    path=DIR,
    src_files=[DIR / 'lame3.70' / f for f in
               ['main.c', 'brhist.c', 'formatBitstream.c', 'fft.c', 'get_audio.c', 'l3bitstream.c', 'id3tag.c',
                'ieeefloat.c', 'lame.c', 'newmdct.c', 'parse.c', 'portableio.c', 'psymodel.c', 'quantize.c',
                'quantize-pvt.c', 'vbrquantize.c', 'reservoir.c', 'tables.c', 'takehiro.c', 'timestatus.c', 'util.c',
                'VbrTag.c', 'version.c', 'gtkanal.c', 'gpkplotting.c', 'mpglib/common.c', 'mpglib/dct64_i386.c',
                'mpglib/decode_i386.c', 'mpglib/layer3.c', 'mpglib/tabinit.c', 'mpglib/interface.c', 'mpglib/main.c']],
    build_flags=['-DHAVEMPGLIB', '-DLAMEPARSE', '-DNDEBUG', '-D__NO_MATH_INLINES', '-O', '-DLAMESNDFILE'],
    workloads=[Workload(None, [DIR / 'large.wav', DIR / 'output_large.mp3'], None, None)]
)
