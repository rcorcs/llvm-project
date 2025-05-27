import os
from pathlib import Path
from llvm import LLVM

# SQLite
from benchmarks.sqlite.benchmark import benchmark as sqlite
# MiBench
from benchmarks.mibench.automotive.basicmath.benchmark import benchmark as basicmath
from benchmarks.mibench.automotive.bitcount.benchmark import benchmark as bitcount
from benchmarks.mibench.automotive.qsort.benchmark import benchmark as qsort
from benchmarks.mibench.automotive.susan.benchmark import benchmark as susan
from benchmarks.mibench.consumer.jpeg.benchmark import cjpeg, djpeg
from benchmarks.mibench.consumer.lame.benchmark import benchmark as lame
from benchmarks.mibench.consumer.tiff.benchmark import tiff2bw, tiff2rgba, tiff2ps, fax2ps, fax2tiff, gif2tiff, pal2rgb, \
    ppm2tiff, ras2tiff, rgb2ycbcr, tiffcmp, tiffcp, tiffdither, tiffdump, tiffinfo, tiffmedian, tiffsplit

from benchmarks.mibench.consumer.typeset.benchmark import benchmark as typeset


def main():
    llvm_dir = Path(os.environ['LLVM_DIR'])
    llvm = LLVM(llvm_dir)

    benchmarks = [
        sqlite,
        basicmath,
        bitcount,
        qsort,
        susan,
        cjpeg,
        djpeg,
        lame,
        tiff2bw,
        tiff2rgba,
        tiff2ps,
        tiffcmp,
        tiffcp,
        tiffdither,
        tiffdump,
        tiffinfo,
        tiffmedian,
        tiffsplit,
        fax2ps,
        fax2tiff,
        gif2tiff,
        pal2rgb,
        ppm2tiff,
        ras2tiff,
        rgb2ycbcr,
        typeset
    ]

    sizes = []
    for benchmark in benchmarks:
        print(f'[benchmark] {benchmark.name}')

        print(f'[emit-ir] {benchmark.ir}')
        benchmark.emit_ir(llvm)

        # print(f'[run]')
        # benchmark.run(llvm)

        def run_opt_then_size(benchmark, flags):
            print(f'[optimize] {flags}')
            opt_ir = benchmark.ir.with_suffix('.opt.ll')
            llvm.optimize(benchmark.ir, opt_ir, flags)
            print(f'[codegen]')
            obj = opt_ir.with_suffix('.o')
            llvm.codegen(opt_ir, obj)
            return llvm.code_size(obj)

        constant_args_flags = ['-passes=func-cloning', '-fe-constants-only=true']
        before_size = run_opt_then_size(benchmark, constant_args_flags)

        cloning_flags = ['-passes=func-cloning', '-fe-constants-only=false']
        after_size = run_opt_then_size(benchmark, cloning_flags)

        print(f'[objsize] {before_size} --> {after_size}')

        sizes.append((before_size, after_size))

    print(f'{"Benchmark":>15} {"constargs":>10} {"cloning":>10} {"diff(Bytes)":>12} {"diff(%)":>8}')
    for benchmark, (before_size, after_size) in zip(benchmarks, sizes):
        diff = -(after_size - before_size)
        print(f'{benchmark.name:>15} {before_size:>10} {after_size:>10} {diff :>12} {100 * diff / before_size:>8.2f}')

    with open('sizes.csv', 'w') as f:
        f.write('benchmark,before,after\n')
        for benchmark, (before_size, after_size) in zip(benchmarks, sizes):
            f.write(f'{benchmark.name},{before_size},{after_size}\n')

    print('Sizes written to sizes.csv')


if __name__ == '__main__':
    main()
