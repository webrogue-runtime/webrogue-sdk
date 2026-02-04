set -ex

if [ "${WEBROGUE_SDK_DEBUG:-0}" = "1" ]; then
    CONFIG=Debug
else
    CONFIG=Release
fi

SDK_ROOT="$(pwd)"
CONFIG_LOWERCASED=$(echo "$CONFIG" | tr '[:upper:]' '[:lower:]')

if [ "${WEBROGUE_SDK_UNCACHED:-0}" = "1" ]; then
    DEST_DIR_APPEND=
else
    DEST_DIR_APPEND=/cached
fi

make -C libraries TOOLCHAIN=wasip1 CONFIG=$CONFIG CONFIG_LOWERCASED=$CONFIG_LOWERCASED DEST_DIR_APPEND=$DEST_DIR_APPEND #1>/dev/null

# these version strings must be kept in sync with builder/Dockerfile
SDK_VERSION=28.0
SDK_MAJOR_VERSION=28

for SDK in x86_64-linux arm64-linux x86_64-windows x86_64-macos arm64-macos
do
    cd $SDK_ROOT/package
    test -d wasi-sdk-$SDK_VERSION-$SDK || {
        curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-$SDK_MAJOR_VERSION/wasi-sdk-$SDK_VERSION-$SDK.tar.gz | tar -xz
    }
    rm -rf webrogue-sdk-$SDK
    cp -r wasi-sdk-$SDK_VERSION-$SDK webrogue-sdk-$SDK
    cd $SDK_ROOT

    for VERSION_TO_REMOVE in wasm32-wasi wasm32-wasi-threads wasm32-wasip1 wasm32-wasip2
    do
        for DIR_TO_REMOVE in include lib share
        do
            rm -rf package/webrogue-sdk-$SDK/share/wasi-sysroot/$DIR_TO_REMOVE/$VERSION_TO_REMOVE
        done
        rm -f package/webrogue-sdk-$SDK/bin/$VERSION_TO_REMOVE-clang
        rm -f package/webrogue-sdk-$SDK/bin/$VERSION_TO_REMOVE-clang++
        rm -f package/webrogue-sdk-$SDK/bin/$VERSION_TO_REMOVE-clang.exe
        rm -f package/webrogue-sdk-$SDK/bin/$VERSION_TO_REMOVE-clang++.exe
    done

    for VERSION_TO_MOVE in wasm32-wasip1-threads
    do
        rm -rf package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/llvm-lto # TODO add lto

        mkdir -p package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/include
        cp -r opt/wasip1/include/* package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/include

        mkdir -p package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/lib
        cp -r opt/wasip1/lib/* package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/lib

        llvm-ar qLs package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/libc++abi.a package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/lib/libcxxemulatedthrow.a
        rm package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/lib/libcxxemulatedthrow.a
        
        llvm-ar qLs package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/libc.a package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/libsetjmp.a
        echo '!<arch>' >package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/libsetjmp.a
    
        # CMAKE_DIR_PATH=package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/lib/cmake/$CMAKE_TARGETS_TO_PATCH
        # CMAKE_TARGETS_FILES_TO_PATCH="
        #     $CMAKE_DIR_PATH/glfw3/glfw3Targets
        #     $CMAKE_DIR_PATH/SDL2/SDL2testTargets
        #     $CMAKE_DIR_PATH/SDL2/SDL2staticTargets
        #     $CMAKE_DIR_PATH/SDL2/SDL2mainTargets
        #     $CMAKE_DIR_PATH/SDL3/SDL3staticTargets
        #     $CMAKE_DIR_PATH/SDL3/SDL3testTargets
        #     $CMAKE_DIR_PATH/SDL2_ttf/SDL2_ttf-static-targets
        #     $CMAKE_DIR_PATH/SDL3_ttf/SDL3_ttf-static-targets
        #     $CMAKE_DIR_PATH/SampleRate/SampleRateTargets
        # "

        # CMAKE_TARGETS_FILES_TO_PATCH="
        #     $CMAKE_DIR_PATH/glfw3/glfw3Targets
        #     $CMAKE_DIR_PATH/SDL2/SDL2testTargets
        #     $CMAKE_DIR_PATH/SDL2/SDL2staticTargets
        #     $CMAKE_DIR_PATH/SDL2/SDL2mainTargets
        # "

        # for CMAKE_TARGETS_FILE_TO_PATCH in $CMAKE_TARGETS_FILES_TO_PATCH
        # do
        #     sed -i "s/\${_IMPORT_PREFIX}\\/lib\\//\${_IMPORT_PREFIX}\\/$VERSION_TO_MOVE\\//g" $CMAKE_TARGETS_FILE_TO_PATCH-$CONFIG_LOWERCASED.cmake
        #     sed -i "s/\${_IMPORT_PREFIX}\\/include/\${_IMPORT_PREFIX}\\/..\\/include\\/$VERSION_TO_MOVE/g" $CMAKE_TARGETS_FILE_TO_PATCH.cmake
        # done
    done

    rm package/webrogue-sdk-$SDK/share/cmake/wasi-sdk-p1.cmake
    rm package/webrogue-sdk-$SDK/share/cmake/wasi-sdk-p2.cmake
    rm package/webrogue-sdk-$SDK/share/cmake/wasi-sdk-pthread.cmake
    rm package/webrogue-sdk-$SDK/share/cmake/wasi-sdk.cmake
    cp opt/wasi-sdk-p1-pthread.cmake package/webrogue-sdk-$SDK/share/cmake/wasi-sdk-p1-pthread.cmake
done
