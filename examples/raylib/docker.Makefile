raylib_:
	cmake -S . -B build --toolchain=/opt/webrogue-sdk/share/cmake/wasi-sdk-p1-pthread.cmake -DCMAKE_BUILD_TYPE=Release
	cmake --build build --target=webrogue_raylib_example --parallel

clean:
	rm -rf build
