raylib_:
	cmake -S . -B build --toolchain=/sdk_dir/package/webrogue-sdk/share/cmake/wasi-sdk-p3.cmake -DCMAKE_BUILD_TYPE=Release
	cmake --build build --target=webrogue_raylib_example --parallel

clean:
	rm -rf build
