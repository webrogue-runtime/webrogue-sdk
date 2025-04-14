set -ex
cd $(dirname $0)

sh builder/build.sh
cp opt/webrogue package/webrogue

test -d package/wasi-x86_64-linux || {
    curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-25/wasi-sdk-25.0-x86_64-linux.tar.gz | tar -xz && mv wasi-sdk-25.0-x86_64-linux package/wasi-x86_64-linux
}
rm -rf package/webrogue-x86_64-linux
cp -r package/wasi-x86_64-linux package/webrogue-x86_64-linux

test -d package/wasi-x86_64-windows || {
    curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-25/wasi-sdk-25.0-x86_64-windows.tar.gz | tar -xz && mv wasi-sdk-25.0-x86_64-windows package/wasi-x86_64-windows
}
rm -rf package/webrogue-x86_64-windows
cp -r package/wasi-x86_64-windows package/webrogue-x86_64-windows

for SDK in x86_64-linux x86_64-windows
do
    for VERSION_TO_REMOVE in wasm32-wasi wasm32-wasi-threads wasm32-wasip1 wasm32-wasip2
    do
        for DIR_TO_REMOVE in include lib share
        do
            rm -rf package/webrogue-$SDK/share/wasi-sysroot/$DIR_TO_REMOVE/$VERSION_TO_REMOVE
        done
        rm -f package/webrogue-$SDK/bin/$VERSION_TO_REMOVE-clang
        rm -f package/webrogue-$SDK/bin/$VERSION_TO_REMOVE-clang++
        rm -f package/webrogue-$SDK/bin/$VERSION_TO_REMOVE-clang.exe
        rm -f package/webrogue-$SDK/bin/$VERSION_TO_REMOVE-clang++.exe
    done

    for VERSION_TO_MOVE in wasm32-wasip1-threads
    do
        rm -rf  package/webrogue-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/llvm-lto # TODO add lto
        cp -r opt/wasip1/include/* package/webrogue-$SDK/share/wasi-sysroot/include/$VERSION_TO_MOVE
        cp -r opt/wasip1/lib/* package/webrogue-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE
        CMAKE_DIR_PATH=package/webrogue-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/cmake/$CMAKE_TARGETS_TO_PATCH
        CMAKE_TARGETS_FILES_TO_PATCH="
            $CMAKE_DIR_PATH/glfw3/glfw3Targets
            $CMAKE_DIR_PATH/SDL2/SDL2testTargets
            $CMAKE_DIR_PATH/SDL2/SDL2staticTargets
            $CMAKE_DIR_PATH/SDL2/SDL2mainTargets
        "

        for CMAKE_TARGETS_FILE_TO_PATCH in $CMAKE_TARGETS_FILES_TO_PATCH
        do
            sed -i "s/\${_IMPORT_PREFIX}\\/lib\\//\${_IMPORT_PREFIX}\\/$VERSION_TO_MOVE\\//g" $CMAKE_TARGETS_FILE_TO_PATCH-release.cmake
            sed -i "s/\${_IMPORT_PREFIX}\\/include/\${_IMPORT_PREFIX}\\/..\\/include\\/$VERSION_TO_MOVE/g" $CMAKE_TARGETS_FILE_TO_PATCH.cmake
        done
    done

    rm package/webrogue-$SDK/share/cmake/wasi-sdk-p1.cmake
    rm package/webrogue-$SDK/share/cmake/wasi-sdk-p2.cmake
    rm package/webrogue-$SDK/share/cmake/wasi-sdk-pthread.cmake
    rm package/webrogue-$SDK/share/cmake/wasi-sdk.cmake
    cp wasi-sdk-p1-pthread.cmake package/webrogue-$SDK/share/cmake/wasi-sdk-p1-pthread.cmake
done

docker build -t webrogue-sdk package
