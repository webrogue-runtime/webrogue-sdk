set -ex
cd "$(dirname "$0")"


# these version strings must be kept in sync with builder/Dockerfile
SDK_VERSION=34.0
SDK_MAJOR_VERSION=34

for SDK in x86_64-linux arm64-linux x86_64-windows x86_64-macos arm64-macos
do
    if [ ! -d "wasi-sdk-$SDK_VERSION-$SDK" ] || [ ! -d "wasi-sdk-$SDK" ]; then
        curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-$SDK_MAJOR_VERSION/wasi-sdk-$SDK_VERSION-$SDK.tar.gz | tar -xz

        rm -rf wasi-sdk-$SDK_VERSION-$SDK/share/wasi-sysroot

        for VERSION_TO_REMOVE in wasm32-wasip1 wasm32-wasip1-threads wasm32-wasip2
        do
            rm -f wasi-sdk-$SDK_VERSION-$SDK/bin/$VERSION_TO_REMOVE-clang
            rm -f wasi-sdk-$SDK_VERSION-$SDK/bin/$VERSION_TO_REMOVE-clang++
            rm -f wasi-sdk-$SDK_VERSION-$SDK/bin/$VERSION_TO_REMOVE-clang.exe
            rm -f wasi-sdk-$SDK_VERSION-$SDK/bin/$VERSION_TO_REMOVE-clang++.exe
        done

        rm -rf wasi-sdk-$SDK
        cp -r wasi-sdk-$SDK_VERSION-$SDK wasi-sdk-$SDK
    fi
done
