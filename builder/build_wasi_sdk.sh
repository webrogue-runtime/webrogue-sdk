set -ex
cd "$(dirname "$0")"

# Building the toolchain from source takes hours. The prebuilt toolchain in
# package/wasi-sdk-prebuilt is built from the same sources (see its VERSION
# file), so it can be reused: set WEBROGUE_SDK_PREBUILT_WASI_SDK=1 to skip the
# toolchain build. The sysroot is still built from source because it differs.
if [ "${WEBROGUE_SDK_PREBUILT_WASI_SDK:-0}" = "1" ]; then
    case "$(uname -s)-$(uname -m)" in
        Linux-x86_64) PREBUILT_SDK=x86_64-linux ;;
        Linux-aarch64 | Linux-arm64) PREBUILT_SDK=arm64-linux ;;
        Darwin-x86_64) PREBUILT_SDK=x86_64-macos ;;
        Darwin-arm64) PREBUILT_SDK=arm64-macos ;;
        *) echo "unsupported host: $(uname -s)-$(uname -m)" >&2; exit 1 ;;
    esac

    PREBUILT_DIR=../package/wasi-sdk-prebuilt/wasi-sdk-$PREBUILT_SDK
    test -d "$PREBUILT_DIR"
    rm -rf wasi-sdk/build/install
    mkdir -p wasi-sdk/build/install
    cp -a "$PREBUILT_DIR"/. wasi-sdk/build/install/
else
    cmake -G Ninja -B wasi-sdk/build/toolchain -S wasi-sdk \
        -DWASI_SDK_BUILD_TOOLCHAIN=ON \
        -DCMAKE_INSTALL_PREFIX=wasi-sdk/build/install \
        -DWASI_SDK_TARGETS=wasm32-wasip3 \

    cmake --build wasi-sdk/build/toolchain --target install
fi

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
wasi-sdk/build/install/bin/llvm-ar qLs wasi-sysroot/lib/wasm32-wasip3/libc.a wasi-sysroot/lib/wasm32-wasip3/libsetjmp.a
echo '!<arch>' >wasi-sysroot/lib/wasm32-wasip3/libsetjmp.a
rm wasi-sysroot/lib/wasm32-wasip3/*.so

rm -rf ../package/wasi-sysroot
cp -r wasi-sysroot ../package/wasi-sysroot
