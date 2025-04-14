set -ex

TOOLS="ar c++filt clang clang++ clang-19 clang-apply-replacements clang-cl
 clang-cpp clang-format clang-tidy git-clang-format ld.lld ld64.lld lld
 lld-link llvm-ar llvm-config llvm-cxxfilt llvm-dwarfdump llvm-dwp llvm-mc
 llvm-nm llvm-objcopy llvm-objdump llvm-ranlib llvm-size llvm-strings llvm-strip
 nm objcopy objdump ranlib run-clang-tidy size strings strip wasm-component-ld
 wasm-ld wasm32-wasip1-threads-clang wasm32-wasip1-threads-clang++"

for TOOL in $TOOLS
do
    ln --symbolic /opt/wasi-sdk/bin/$TOOL /opt/wasip1/bin/$TOOL
done
