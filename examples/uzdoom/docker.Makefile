CURRENT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

build_uzdoom: build_native_uzdoom freedoom-0.13.0
	cmake -S UZDoom -B build --toolchain=/sdk_dir/package/webrogue-sdk/share/cmake/wasi-sdk-p3.cmake -DCMAKE_BUILD_TYPE=Release -DIMPORT_EXECUTABLES=$(CURRENT_DIR)/build_native/ImportExecutables.cmake -DVULKAN_USE_XLIB=OFF -DVULKAN_USE_WAYLAND=OFF -DZMUSIC_BUILD_SHARED_LIBS=OFF -DNO_OPENAL=ON -DFT_DISABLE_BZIP2=ON
	cmake --build build --target uzdoom
	
build_native_uzdoom:
	(unset PKG_CONFIG_PATH; unset PKG_CONFIG_LIBDIR; cmake -S UZDoom -B build_native -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DNO_OPENAL=ON)
	cmake --build build_native --target uzdoom uzdoom_pk3 game_support_pk3

freedoom-0.13.0:
	wget https://github.com/freedoom/freedoom/releases/download/v0.13.0/freedoom-0.13.0.zip -O freedoom-0.13.0.zip
	unzip freedoom-0.13.0

clean:
	rm -rf build build_native
