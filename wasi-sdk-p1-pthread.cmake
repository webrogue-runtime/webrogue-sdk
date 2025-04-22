# Cmake toolchain description file for the Makefile

set(CMAKE_SYSTEM_NAME WASI)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR wasm32)
set(triple wasm32-wasip1-threads)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
# wasi-threads requires --import-memory.
# wasi requires --export-memory.
# (--export-memory is implicit unless --import-memory is given)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--import-memory")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--export-memory")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--max-memory=4294967296")

if(WIN32)
	set(WASI_HOST_EXE_SUFFIX ".exe")
else()
	set(WASI_HOST_EXE_SUFFIX "")
endif()

# When building from source, WASI_SDK_PREFIX represents the generated directory
if(NOT WASI_SDK_PREFIX)
    set(WASI_SDK_PREFIX ${CMAKE_CURRENT_LIST_DIR}/../../)
endif()

# Just for clangd
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${WASI_SDK_PREFIX}/share/wasi-sysroot/include/${triple}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${WASI_SDK_PREFIX}/share/wasi-sysroot/include/${triple}/c++/v1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${WASI_SDK_PREFIX}/share/wasi-sysroot/include/${triple}")

set(CMAKE_C_COMPILER ${WASI_SDK_PREFIX}/bin/clang${WASI_HOST_EXE_SUFFIX})
set(CMAKE_CXX_COMPILER ${WASI_SDK_PREFIX}/bin/clang++${WASI_HOST_EXE_SUFFIX})
set(CMAKE_ASM_COMPILER ${WASI_SDK_PREFIX}/bin/clang${WASI_HOST_EXE_SUFFIX})
set(CMAKE_AR ${WASI_SDK_PREFIX}/bin/llvm-ar${WASI_HOST_EXE_SUFFIX})
set(CMAKE_RANLIB ${WASI_SDK_PREFIX}/bin/llvm-ranlib${WASI_HOST_EXE_SUFFIX})
list(APPEND CMAKE_PREFIX_PATH ${WASI_SDK_PREFIX}/share/wasi-sysroot/lib/${triple}/cmake)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_ASM_COMPILER_TARGET ${triple})

# Don't look in the sysroot for executables to run during the build
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Only look in the sysroot (not in the host paths) for the rest
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_INCLUDE_PATH ${WASI_SDK_PREFIX}/share/wasi-sysroot/include/${triple})
set(CMAKE_LIBRARY_PATH ${WASI_SDK_PREFIX}/share/wasi-sysroot/lib/${triple})
