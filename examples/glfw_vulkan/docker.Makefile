glfw:
	cmake -S . -B build --toolchain=/sdk_dir/package/webrogue-sdk/share/cmake/wasi-sdk-p1-pthread.cmake -DCMAKE_BUILD_TYPE=Debug  -DGLFW_BUILD_EXAMPLES=ON -DGLFW_BUILD_WEBROGUE=ON
	cmake --build build --target=main --parallel

clean:
	rm -rf build
