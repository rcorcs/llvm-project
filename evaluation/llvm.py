import os
import subprocess
from pathlib import PosixPath
from sys import platform, stdout, stdin


class LLVM:
    def __init__(self, llvm_dir):
        self.clang = [llvm_dir / 'bin' / 'clang']
        self.clangpp = [llvm_dir / 'bin' / 'clang++']
        if platform == 'darwin':
            self.clang.append(f'-isysroot{os.popen('xcrun --show-sdk-path').read().strip()}')
            self.clangpp.append(f'-isysroot{os.popen('xcrun --show-sdk-path').read().strip()}')
        self.opt = [llvm_dir / 'bin' / 'opt']
        self.llc = [llvm_dir / 'bin' / 'llc']
        self.llvm_link = [llvm_dir / 'bin' / 'llvm-link']
        self.llvm_objdump = [llvm_dir / 'bin' / 'llvm-objdump']
        self.llvm_size = [llvm_dir / 'bin' / 'llvm-size']
        self.llvm_dis = [llvm_dir / 'bin' / 'llvm-dis']
        self.lli = [llvm_dir / 'bin' / 'lli']

    # takes input and output file paths
    def emit_ir(self, src_file, ir_file, options=None):
        if options is None:
            options = []
        assert ir_file.suffix == '.ll', f'Output file must have .ll extension, got {ir_file.suffix}'
        flags = ['-Os', '-fno-vectorize', '-fno-slp-vectorize', '-fno-unroll-loops', '-fno-inline-functions', '-Wno-implicit-function-declaration', '-Wno-implicit-int']
        compiler = self.clang if src_file.suffix == '.c' else self.clangpp
        cmd = [str(s) for s in compiler] + flags + ['-S', '-emit-llvm'] + options + [str(src_file), '-o',
                                                                                     str(ir_file)]
        print(' '.join(cmd))
        subprocess.run(cmd, capture_output=True, text=True, check=True)

    def link(self, ir_files, dest_file):
        assert dest_file.suffix == '.ll', f'Output file must have .ll extension, got {dest_file.suffix}'
        cmd = [str(s) for s in self.llvm_link] + ['-S', '-o', str(dest_file)] + [str(ir) for ir in ir_files]
        print(' '.join(cmd))
        subprocess.run(cmd, capture_output=True, text=True, check=True)

    def optimize(self, ir_file, optimized_ir_file, options=None):
        if options is None:
            options = []
        assert optimized_ir_file.suffix == '.ll', f'Output file must have .ll extension, got {optimized_ir_file.suffix}'
        cmd = [str(s) for s in self.opt] + ['-S', '-o', str(optimized_ir_file)] + options + [str(ir_file)]
        print(' '.join(cmd))
        out = subprocess.run(cmd, capture_output=True, text=True, check=True)
        # print(out.stderr)

    def codegen(self, ir_file, obj_file, options=None):
        if options is None:
            options = []
        options += ['-code-model=small']
        # options += ['-code-model=small', '-mattr=+compact-branches', '-relocation-model=pic']
        assert obj_file.suffix == '.o', f'Output file must have .o extension, got {obj_file.suffix}'
        cmd = [str(s) for s in self.llc] + ['-filetype=obj', '-o', str(obj_file)] + options + [str(ir_file)]
        print(' '.join(cmd))
        subprocess.run(cmd, capture_output=True, text=True, check=True)

    def objdump(self, obj_file):
        cmd = [str(s) for s in self.llvm_objdump] + ['-d', str(obj_file)]
        print(' '.join(cmd))
        out = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return out.stdout

    def code_size(self, obj_file):
        cmd = [str(s) for s in self.llvm_size] + [str(obj_file)]
        print(' '.join(cmd))
        out = subprocess.run(cmd, capture_output=True, text=True, check=True)
        print(out.stdout)

        lines = out.stdout.splitlines()
        line = lines[1]
        parts = line.split('\t')
        text_size = int(parts[0])
        return text_size

    def interpret(self, ir_file, workload):
        assert ir_file.suffix == '.ll', f'Input file must have .ll extension, got {ir_file.suffix}'

        in_file = open(workload.input, 'r') if workload.input else None
        out_file = open(workload.output, 'w') if workload.output else None

        cmd = [str(arg) for arg in self.lli] + [str(ir_file)] + [str(arg) for arg in workload.args]
        print(' '.join(cmd))
        subprocess.run(cmd, check=True, stdin=in_file, stdout=out_file)
