set -ex
cd "$(dirname "$0")"

cmake -G Ninja -B wasi-sdk/build/toolchain -S wasi-sdk \
    -DWASI_SDK_BUILD_TOOLCHAIN=ON \
    -DCMAKE_INSTALL_PREFIX=wasi-sdk/build/install \
    -DWASI_SDK_TARGETS=wasm32-wasip3 \

cmake --build wasi-sdk/build/toolchain --target install

cmake -G Ninja -B wasi-sdk/build/sysroot -S wasi-sdk \
    -DCMAKE_INSTALL_PREFIX=wasi-sdk/build/install \
    -DCMAKE_TOOLCHAIN_FILE=build/install/share/cmake/wasi-sdk-p3.cmake \
    -DCMAKE_C_COMPILER_WORKS=ON \
    -DCMAKE_CXX_COMPILER_WORKS=ON \
    -DWASI_SDK_INSTALL_TO_CLANG_RESOURCE_DIR=ON \
    -DWASI_SDK_CPU_CFLAGS="" \
    -DWASI_SDK_TARGETS=wasm32-wasip3 \
    -DWASI_SDK_EXCEPTIONS=ON \

cmake --build wasi-sdk/build/sysroot --target install

rm -rf wasi-sysroot
mv wasi-sdk/build/install/share/wasi-sysroot/experimental-coop-threads wasi-sysroot
llvm-ar qLs wasi-sysroot/lib/wasm32-wasip3/libc.a wasi-sysroot/lib/wasm32-wasip3/libsetjmp.a
echo '!<arch>' >wasi-sysroot/lib/wasm32-wasip3/libsetjmp.a
rm wasi-sysroot/lib/wasm32-wasip3/*.so

rm -rf ../package/wasi-sysroot
cp -r wasi-sysroot ../package/wasi-sysroot
