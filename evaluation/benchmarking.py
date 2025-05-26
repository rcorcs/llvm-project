class Workload:
    def __init__(self, name, args, input_file, output_file):
        self.name = name
        self.args = args
        self.input = input_file
        self.output = output_file


class Benchmark:
    def __init__(self, name, path, src_files, build_flags, workloads):
        self.name = name
        self.path = path
        self.src_files = src_files
        self.build_flags = build_flags
        self.workloads = workloads
        self.ir = (path / name).with_suffix('.ll')

    def emit_ir(self, llvm):
        irs = []
        for file in self.src_files:
            out = file.with_suffix('.ll')
            llvm.emit_ir(file, out, self.build_flags)
            irs.append(out)

        tmp = self.ir.with_suffix('.tmp.ll')
        llvm.link(irs, tmp)

        [ir.unlink() for ir in irs]

        tmp.rename(self.ir)

    def run(self, llvm):
        for workload in self.workloads:
            llvm.interpret(self.ir, workload)
