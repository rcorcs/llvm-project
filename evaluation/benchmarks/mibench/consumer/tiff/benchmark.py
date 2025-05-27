import glob
from pathlib import Path
from benchmarking import Benchmark, Workload

DIR = Path(__file__).parent

libs = ['libtiff/tif_aux.c', 'libtiff/tif_close.c', 'libtiff/tif_codec.c', 'libtiff/tif_compress.c',
        'libtiff/tif_dir.c', 'libtiff/tif_dirinfo.c', 'libtiff/tif_dirread.c', 'libtiff/tif_dirwrite.c',
        'libtiff/tif_dumpmode.c', 'libtiff/tif_error.c', 'libtiff/tif_fax3.c', 'libtiff/tif_fax3sm.c',
        'libtiff/tif_getimage.c', 'libtiff/tif_jpeg.c', 'libtiff/tif_flush.c', 'libtiff/tif_luv.c',
        'libtiff/tif_lzw.c', 'libtiff/tif_next.c', 'libtiff/tif_open.c', 'libtiff/tif_packbits.c',
        'libtiff/tif_pixarlog.c', 'libtiff/tif_predict.c', 'libtiff/tif_print.c', 'libtiff/tif_read.c',
        'libtiff/tif_swab.c', 'libtiff/tif_strip.c', 'libtiff/tif_thunder.c', 'libtiff/tif_tile.c',
        'libtiff/tif_unix.c', 'libtiff/tif_version.c', 'libtiff/tif_warning.c', 'libtiff/tif_write.c',
        'libtiff/tif_zip.c']

# tiff2bw
tiff2bw = Benchmark(
    name='tiff2bw',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiff2bw.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[
        Workload(None,
                 [
                     DIR / 'tiff-data/large.tif',
                     DIR / 'output_largebw.tif'
                 ],
                 None,
                 None),
    ]
)

# tiff2rgba
tiff2rgba = Benchmark(
    name='tiff2rgba',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiff2rgba.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[
        Workload(None,
                 [
                     DIR / 'tiff-data/large.tif',
                     DIR / 'output_large.tif'
                 ],
                 None,
                 None),
    ]
)

# tiff2ps
tiff2ps = Benchmark(
    name='tiff2ps',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiff2ps.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[
        Workload(None,
                 [
                     DIR / 'tiff-data/large.tif',
                     DIR / 'output_large.ps'
                 ],
                 None,
                 None),
    ]
)

# fax2ps
fax2ps = Benchmark(
    name='fax2ps',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/fax2ps.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)

# fax2tiff
fax2tiff = Benchmark(
    name='fax2tiff',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/fax2tiff.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)

# gif2tiff
gif2tiff = Benchmark(
    name='gif2tiff',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/gif2tiff.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)

# pal2rgb
pal2rgb = Benchmark(
    name='pal2rgb',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/pal2rgb.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# ppm2tiff
ppm2tiff = Benchmark(
    name='ppm2tiff',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/ppm2tiff.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# ras2tiff
ras2tiff = Benchmark(
    name='ras2tiff',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/ras2tiff.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# rgb2ycbcr
rgb2ycbcr = Benchmark(
    name='rgb2ycbcr',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/rgb2ycbcr.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# tiffcmp
tiffcmp = Benchmark(
    name='tiffcmp',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiffcmp.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# tiffcp
tiffcp = Benchmark(
    name='tiffcp',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiffcp.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# tiffdither
tiffdither = Benchmark(
    name='tiffdither',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiffdither.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# tiffdump
tiffdump = Benchmark(
    name='tiffdump',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiffdump.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# tiffinfo
tiffinfo = Benchmark(
    name='tiffinfo',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiffinfo.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# tiffmedian
tiffmedian = Benchmark(
    name='tiffmedian',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiffmedian.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
# tiffsplit
tiffsplit = Benchmark(
    name='tiffsplit',
    path=DIR,
    src_files=[DIR / 'tiff-v3.5.4' / f for f in libs + ['tools/tiffsplit.c']],
    build_flags=[
        f'-I{str(DIR / "tiff-v3.5.4")}',
        f'-I{str(DIR / "tiff-v3.5.4" / "libtiff")}',
    ],
    workloads=[]
)
