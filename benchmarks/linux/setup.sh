#MANUALLY OVERRIDE
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
LLVM_DIR=$(realpath $(SCRIPT_DIR)/../../llvm-project/build/release/bin)/
git clone --depth 1 https://github.com/torvalds/linux.git --branch v5.19
patch -p0 -R < patch.txt
mkdir build
make LLVM=${LLVM_DIR} defconfig
#make LLVM=${LLVM_DIR} vmlinux
