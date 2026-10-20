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

export CMAKE_GENERATOR=Ninja
make -C libraries TOOLCHAIN=wasip3 CONFIG=$CONFIG CONFIG_LOWERCASED=$CONFIG_LOWERCASED DEST_DIR_APPEND=$DEST_DIR_APPEND

for SDK in x86_64-linux arm64-linux x86_64-windows x86_64-macos arm64-macos
do
    cd $SDK_ROOT/package
    rm -rf webrogue-sdk-$SDK
    cp -r wasi-sdk-prebuilt/wasi-sdk-$SDK webrogue-sdk-$SDK
    ls
    cp -r wasi-sysroot webrogue-sdk-$SDK/share
    cd $SDK_ROOT

    for VERSION_TO_MOVE in wasm32-wasip3
    do
        # TODO add lto
        rm -rf package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/llvm-lto 

        mkdir -p package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/include
        cp -r opt/wasip3/include/* package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/include

        mkdir -p package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/lib
        cp -r opt/wasip3/lib/* package/webrogue-sdk-$SDK/share/webrogue-sysroot/$VERSION_TO_MOVE/lib

        llvm-ar qLs package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/libc++abi.a package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/libunwind.a
        echo '!<arch>' > package/webrogue-sdk-$SDK/share/wasi-sysroot/lib/$VERSION_TO_MOVE/libunwind.a
        

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
    rm package/webrogue-sdk-$SDK/share/cmake/wasi-sdk-p3.cmake
    rm package/webrogue-sdk-$SDK/share/cmake/wasi-sdk-pthread.cmake
    cp opt/wasi-sdk-p3.cmake package/webrogue-sdk-$SDK/share/cmake/wasi-sdk-p3.cmake
done
