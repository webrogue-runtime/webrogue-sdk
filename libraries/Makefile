all: copy_glad copy_gles2 copy_gles3 copy_khr build_webroguegfx build_cxxemulatedthrow build_glfw build_vulkan_headers build_jpeg build_png build_zlib build_libsamplerate build_vorbis build_ogg build_freetype build_harfbuzz build_mbedtls # build_SDL2 build_SDL3 build_SDL2_ttf build_SDL3_ttf build_curl
CURRENT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SDK_DIR:=$(shell dirname $(CURRENT_DIR))
DESTINATION_DIR=$(SDK_DIR)/opt/$(TOOLCHAIN)
BUILD_DIR=$(CURRENT_DIR)/build
CMAKE_FLAGS=--toolchain=$(DESTINATION_DIR)/toolchain.cmake -DWASI_SDK_PREFIX=/wasi-sdk -DCMAKE_BUILD_TYPE=$(CONFIG) -Wno-dev

# copy headers
copy_glad: $(DESTINATION_DIR)/include/glad

$(DESTINATION_DIR)/include/glad:
	mkdir -p $(DESTINATION_DIR)/include/glad 
	cp -r glfw/deps/glad $(DESTINATION_DIR)/include

copy_gles2: $(DESTINATION_DIR)/include/GLES2 

$(DESTINATION_DIR)/include/GLES2:
	mkdir -p $(DESTINATION_DIR)/include/GLES2 
	cp -r GLES2 $(DESTINATION_DIR)/include

copy_gles3: $(DESTINATION_DIR)/include/GLES3

$(DESTINATION_DIR)/include/GLES3:
	mkdir -p $(DESTINATION_DIR)/include/GLES3
	cp -r GLES3/ $(DESTINATION_DIR)/include

copy_khr: $(DESTINATION_DIR)/include/KHR

$(DESTINATION_DIR)/include/KHR:
	mkdir -p $(DESTINATION_DIR)/include/KHR
	cp -r KHR/ $(DESTINATION_DIR)/include

# build and install libs
build_webroguegfx: copy_gles2 copy_gles3 copy_khr build_mesa
	cmake -S webroguegfx -B $(BUILD_DIR)/webroguegfx $(CMAKE_FLAGS) -DMESA_PATH=$(BUILD_DIR)/mesa 
	cmake --build $(BUILD_DIR)/webroguegfx --target=webroguegfx --parallel
	cmake --install $(BUILD_DIR)/webroguegfx --prefix $(DESTINATION_DIR)

build_cxxemulatedthrow:
	cd cxxemulatedthrow && cmake -S . -B $(BUILD_DIR)/cxxemulatedthrow $(CMAKE_FLAGS)
	cmake --build $(BUILD_DIR)/cxxemulatedthrow --target=cxxemulatedthrow --parallel
	cmake --install $(BUILD_DIR)/cxxemulatedthrow --prefix $(DESTINATION_DIR)

build_SDL2: build_webroguegfx build_libsamplerate
	cmake -S SDL2 -B $(BUILD_DIR)/SDL2 $(CMAKE_FLAGS) -DSDL_STATIC=ON -DSDL_SHARED=OFF
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2-static --parallel
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2main --parallel
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2_test --parallel
	cmake --install $(BUILD_DIR)/SDL2 --prefix $(DESTINATION_DIR)

build_SDL2_ttf: build_SDL2 build_freetype
	cmake -S SDL2_ttf -B $(BUILD_DIR)/SDL2_ttf $(CMAKE_FLAGS) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=OFF -DSDL2_DIR=$(DESTINATION_DIR)/lib/cmake/SDL2 -DFREETYPE_INCLUDE_DIRS=$(DESTINATION_DIR)/include/freetype2
	cmake --build $(BUILD_DIR)/SDL2_ttf --target=SDL2_ttf --parallel
	cmake --install $(BUILD_DIR)/SDL2_ttf --prefix $(DESTINATION_DIR)

build_SDL3: build_webroguegfx
	cmake -S SDL3 -B $(BUILD_DIR)/SDL3 $(CMAKE_FLAGS) -DSDL_STATIC=ON -DSDL_SHARED=OFF
	cmake --build $(BUILD_DIR)/SDL3 --target=SDL3-static --parallel
	cmake --build $(BUILD_DIR)/SDL3 --target=SDL3_test --parallel
	cmake --install $(BUILD_DIR)/SDL3 --prefix $(DESTINATION_DIR)

build_SDL3_ttf: build_SDL3 build_freetype
	cmake -S SDL3_ttf -B $(BUILD_DIR)/SDL3_ttf $(CMAKE_FLAGS) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=OFF -DSDL3_DIR=$(DESTINATION_DIR)/lib/cmake/SDL3 -DFREETYPE_INCLUDE_DIRS=$(DESTINATION_DIR)/include/freetype2
	cmake --build $(BUILD_DIR)/SDL3_ttf --target=SDL3_ttf-static --parallel
	cmake --install $(BUILD_DIR)/SDL3_ttf --prefix $(DESTINATION_DIR)

build_glfw: build_webroguegfx
	cmake -S glfw -B $(BUILD_DIR)/glfw $(CMAKE_FLAGS) -DGLFW_BUILD_EXAMPLES=ON -DGLFW_BUILD_WEBROGUE=ON
	cmake --build $(BUILD_DIR)/glfw --target=glfw --parallel
	cmake --install $(BUILD_DIR)/glfw --prefix $(DESTINATION_DIR)

build_jpeg: 
	cd jpeg && cmake -S . -B $(BUILD_DIR)/jpeg $(CMAKE_FLAGS)
	cmake --build $(BUILD_DIR)/jpeg --target=jpeg --parallel
	cmake --install $(BUILD_DIR)/jpeg --prefix $(DESTINATION_DIR)

build_png: build_zlib
	cd libpng && cmake -S . -B $(BUILD_DIR)/png $(CMAKE_FLAGS) -DZLIB_LIBRARY=$(DESTINATION_DIR)/lib/libzlib.a -DZLIB_INCLUDE_DIR=$(DESTINATION_DIR)/include -DPNG_SHARED=OFF -DPNG_STATIC=ON -DCMAKE_C_FLAGS="-DPNG_SETJMP_NOT_SUPPORTED"
	cmake --build $(BUILD_DIR)/png --target=png_static --parallel
	cmake --install $(BUILD_DIR)/png --prefix $(DESTINATION_DIR)

build_zlib:
	cd zlib && cmake -S . -B $(BUILD_DIR)/zlib $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DZLIB_SKIP_SHARED=ON
	cmake --build $(BUILD_DIR)/zlib --target=zlibstatic --parallel
	cmake --install $(BUILD_DIR)/zlib --prefix $(DESTINATION_DIR)
	cp $(DESTINATION_DIR)/lib/libzlibstatic.a $(DESTINATION_DIR)/lib/libzlib.a

build_libsamplerate: 
	cd libsamplerate && cmake -S . -B $(BUILD_DIR)/libsamplerate $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/libsamplerate --target=samplerate --parallel
	cmake --install $(BUILD_DIR)/libsamplerate --prefix $(DESTINATION_DIR)

build_vorbis: build_ogg
	cd vorbis && cmake -S . -B $(BUILD_DIR)/vorbis $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DOGG_LIBRARY=$(DESTINATION_DIR)/lib/libogg.a -DOGG_INCLUDE_DIR=$(DESTINATION_DIR)/include/ogg/ogg.h -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/vorbis --target=vorbis --parallel
	cmake --build $(BUILD_DIR)/vorbis --target=vorbisenc --parallel
	cmake --build $(BUILD_DIR)/vorbis --target=vorbisfile --parallel
	cmake --install $(BUILD_DIR)/vorbis --prefix $(DESTINATION_DIR)

build_ogg:
	cd ogg && cmake -S . -B $(BUILD_DIR)/ogg $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/ogg --target=install --parallel

build_freetype: build_zlib build_png
	cd freetype && cmake -S . -B $(BUILD_DIR)/freetype $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DFT_REQUIRE_PNG=ON -DFT_REQUIRE_ZLIB=ON -DPNG_LIBRARY=$(DESTINATION_DIR)/lib/libpng.a -DPNG_PNG_INCLUDE_DIR=$(DESTINATION_DIR)/include/libpng16 -DZLIB_LIBRARY=$(DESTINATION_DIR)/lib/libzlib.a -DZLIB_INCLUDE_DIR=$(DESTINATION_DIR)/include
	cmake --build $(BUILD_DIR)/freetype --target=install --parallel

build_harfbuzz:
	cd harfbuzz && cmake -S . -B $(BUILD_DIR)/harfbuzz $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DHB_HAVE_ICU=OFF -DHB_BUILD_SUBSET=OFF -DHB_HAVE_FREETYPE=ON -DFREETYPE_INCLUDE_DIR_freetype2=$(DESTINATION_DIR)/include/freetype2 -DFREETYPE_INCLUDE_DIR_ft2build=$(DESTINATION_DIR)/include/freetype2 -DFREETYPE_LIBRARY_DEBUG=$(DESTINATION_DIR)/lib/libfreetype.a -DFREETYPE_LIBRARY_RELEASE=$(DESTINATION_DIR)/lib/libfreetype.a
	cmake --build $(BUILD_DIR)/harfbuzz --target=harfbuzz --parallel
	cmake --install $(BUILD_DIR)/harfbuzz --prefix $(DESTINATION_DIR)

build_curl: build_mbedtls
	cd curl && cmake -S . -B $(BUILD_DIR)/curl $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DCURL_USE_MBEDTLS=ON -DMBEDTLS_INCLUDE_DIR=$(DESTINATION_DIR)/include/mbedtls -DMBEDTLS_LIBRARY=$(DESTINATION_DIR)/lib/libmbedtls.a -DMBEDX509_LIBRARY=$(DESTINATION_DIR)/lib/libmbedx509.a -DMBEDCRYPTO_LIBRARY=$(DESTINATION_DIR)/lib/libmbedcrypto.a -DCURL_USE_LIBPSL=OFF -DBUILD_LIBCURL_DOCS=OFF -DBUILD_MISC_DOCS=OFF -DENABLE_CURL_MANUAL=OFF -DBUILD_CURL_EXE=OFF -DENABLE_IPV6=OFF
	cmake --build $(BUILD_DIR)/curl --target=libcurl_static --parallel
	cmake --install $(BUILD_DIR)/curl --prefix $(DESTINATION_DIR)

mbedtls:
	curl -L https://github.com/Mbed-TLS/mbedtls/releases/download/mbedtls-3.6.2/mbedtls-3.6.2.tar.bz2 | tar -xj
	mv mbedtls-3.6.2 mbedtls
	# TODO use diff
	cp mbedtls_patch/mbedtls_config.h mbedtls/include/mbedtls/mbedtls_config.h
	cp mbedtls_patch/timing.c mbedtls/library/timing.c
	cp mbedtls_patch/platform_util.c mbedtls/library/platform_util.c
	cp mbedtls_patch/net_sockets.c mbedtls/library/net_sockets.c

build_mbedtls: mbedtls
	cd mbedtls && cmake -S . -B $(BUILD_DIR)/mbedtls $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTING=Off -DGEN_FILES=OFF -DENABLE_PROGRAMS=OFF
	cmake --build $(BUILD_DIR)/mbedtls --target=mbedtls --parallel
	cmake --install $(BUILD_DIR)/mbedtls --prefix $(DESTINATION_DIR)

build_mesa: build_vulkan_headers
	meson setup --reconfigure $(BUILD_DIR)/mesa mesa \
		-Dgallium-drivers= \
		-Dvulkan-drivers=gfxstream \
		-Dplatforms=webrogue \
		-Dglx=disabled \
		-Dllvm=disabled \
		-Degl-native-platform=surfaceless \
		-Dgles1=disabled \
		-Dgles2=disabled \
		-Dgbm=disabled \
		-Dexpat=disabled \
		-Degl=disabled \
		-Dglvnd=disabled \
		-Dzlib=disabled \
		-Dshader-cache=disabled \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt
	ninja -C $(BUILD_DIR)/mesa \
		src/vulkan/runtime/libvulkan_runtime.a \
		src/vulkan/runtime/libvulkan_instance.a \
		src/gfxstream/guest/vulkan/libvulkan_gfxstream.a \
		src/util/libxmlconfig.a \
		src/util/libparson.a \
		src/compiler/isaspec/libisaspec.a \
		src/vulkan/util/libvulkan_util.a \
		src/vulkan/runtime/libvulkan_lite_runtime.a \
		src/vulkan/runtime/libvulkan_lite_instance.a \
		src/vulkan/runtime/libvulkan_runtime.a \
		src/vulkan/runtime/libvulkan_instance.a \
		src/vulkan/wsi/libvulkan_wsi.a \
		src/gfxstream/guest/platform/libplatform_virtgpu.a \
		src/gfxstream/guest/platform/kumquat/libplatform_virtgpu_kumquat.a \
		src/gfxstream/guest/GoldfishAddressSpace/libgoldfish_address_space.a \
		src/gfxstream/guest/connection-manager/libconnection_manager.a \
		src/gfxstream/guest/vulkan/libvulkan_gfxstream.a \
		src/gfxstream/guest/vulkan/stubs/libgfxstream_vk_stubs.a \
		src/gfxstream/aemu/libaemu.a
		# src/compiler/spirv/spirv2nir
		# src/vulkan/vulkan_api.def: CUSTOM_COMMAND
		# src/gtest/libgtest.a \

build_vulkan_headers:
	cmake -S Vulkan-Headers -B $(BUILD_DIR)/vulkan_headers $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR)
	cmake --install $(BUILD_DIR)/vulkan_headers --prefix $(DESTINATION_DIR)


clean_mbedtls:
	rm -rf mbedtls

clean:
	rm -rf $(BUILD_DIR)
