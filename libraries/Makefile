all: build_volk_in_loaders_cloth copy_pkgconfig copy_gl copy_glad copy_gles2 copy_gles3 copy_egl copy_khr build_libwr4c build_glfw build_vulkan_headers build_jpeg build_png build_zlib build_libsamplerate build_vorbis build_ogg build_freetype build_harfbuzz build_SDL2 build_SDL3 build_SDL2_ttf build_SDL3_ttf build_angle build_mbedtls build_libffi build_cairo build_pango build_gtk build_glib build_fontconfig build_expat build_libxml2 build_gdk-pixbuf build_shaderc # build_curl
CURRENT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SDK_DIR:=$(shell dirname $(CURRENT_DIR))
FINAL_DEST_DIR=$(SDK_DIR)/opt/$(TOOLCHAIN)
DEST_DIR=$(FINAL_DEST_DIR)$(DEST_DIR_APPEND)
BUILD_DIR=$(CURRENT_DIR)/build/wasip3/$(CONFIG)
CMAKE_FLAGS=--toolchain=$(FINAL_DEST_DIR)/toolchain.cmake -DWASI_SDK_PREFIX=/wasi-sdk -DCMAKE_BUILD_TYPE=$(CONFIG) -Wno-dev
MESA_FLAG=--cross-file=$(FINAL_DEST_DIR)/meson_cross.txt -Ddefault_library=static --buildtype=$(CONFIG_LOWERCASED)

copy_pkgconfig:
	mkdir -p $(DEST_DIR)/lib/pkgconfig
	cp -r pkgconfig/*.pc $(DEST_DIR)/lib/pkgconfig
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

# copy headers
copy_gl: $(FINAL_DEST_DIR)/include/GL

$(FINAL_DEST_DIR)/include/GL:
	mkdir -p $(DEST_DIR)/include/
	cp -r GL $(DEST_DIR)/include
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

copy_glad: $(FINAL_DEST_DIR)/include/glad

$(FINAL_DEST_DIR)/include/glad:
	mkdir -p $(DEST_DIR)/include/glad 
	cp -r glfw/deps/glad $(DEST_DIR)/include
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

copy_gles2: $(FINAL_DEST_DIR)/include/GLES2

$(FINAL_DEST_DIR)/include/GLES2:
	mkdir -p $(DEST_DIR)/include/GLES2
	cp -r GLES2 $(DEST_DIR)/include
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

copy_gles3: $(FINAL_DEST_DIR)/include/GLES3

$(FINAL_DEST_DIR)/include/GLES3:
	mkdir -p $(DEST_DIR)/include/GLES3
	cp -r GLES3/ $(DEST_DIR)/include
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

copy_khr: $(FINAL_DEST_DIR)/include/KHR

$(FINAL_DEST_DIR)/include/KHR:
	mkdir -p $(DEST_DIR)/include/KHR
	cp -r KHR/ $(DEST_DIR)/include
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

copy_egl: $(FINAL_DEST_DIR)/include/EGL

$(FINAL_DEST_DIR)/include/EGL:
	mkdir -p $(DEST_DIR)/include/EGL
	cp -r EGL/ $(DEST_DIR)/include
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

# build and install libs
build_libwr4c: copy_gles2 copy_gles3 copy_khr
	cmake -S libwr4c -B $(BUILD_DIR)/libwr4c $(CMAKE_FLAGS) -DMESA_PATH=$(BUILD_DIR)/mesa 
	cmake --build $(BUILD_DIR)/libwr4c --target=wr4c --parallel
	cmake --install $(BUILD_DIR)/libwr4c --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_SDL2: build_libwr4c build_libsamplerate
	cmake -S SDL2 -B $(BUILD_DIR)/SDL2 $(CMAKE_FLAGS) -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_CMAKE_DEBUG_POSTFIX=
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2-static --parallel
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2main --parallel
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2_test --parallel
	cmake --install $(BUILD_DIR)/SDL2 --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_SDL2_ttf: build_SDL2 build_freetype
	cmake -S SDL2_ttf -B $(BUILD_DIR)/SDL2_ttf $(CMAKE_FLAGS) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=OFF -DSDL2_DIR=$(FINAL_DEST_DIR)/lib/cmake/SDL2 -DFREETYPE_INCLUDE_DIRS=$(FINAL_DEST_DIR)/include/freetype2 -DSDL2TTF_DEBUG_POSTFIX=
	cmake --build $(BUILD_DIR)/SDL2_ttf --target=SDL2_ttf --parallel
	cmake --install $(BUILD_DIR)/SDL2_ttf --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_SDL3: build_libwr4c
	cmake -S SDL3 -B $(BUILD_DIR)/SDL3 $(CMAKE_FLAGS) -DSDL_STATIC=ON -DSDL_SHARED=OFF
	cmake --build $(BUILD_DIR)/SDL3 --target=SDL3-static --parallel
	cmake --build $(BUILD_DIR)/SDL3 --target=SDL3_test --parallel
	cmake --install $(BUILD_DIR)/SDL3 --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_SDL3_ttf: build_SDL3 build_freetype
	cmake -S SDL3_ttf -B $(BUILD_DIR)/SDL3_ttf $(CMAKE_FLAGS) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=OFF -DSDL3_DIR=$(FINAL_DEST_DIR)/lib/cmake/SDL3 -DFREETYPE_INCLUDE_DIRS=$(FINAL_DEST_DIR)/include/freetype2
	cmake --build $(BUILD_DIR)/SDL3_ttf --target=SDL3_ttf-static --parallel
	cmake --install $(BUILD_DIR)/SDL3_ttf --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_glfw: build_libwr4c
	cmake -S glfw -B $(BUILD_DIR)/glfw $(CMAKE_FLAGS) -DGLFW_BUILD_EXAMPLES=ON -DGLFW_BUILD_WEBROGUE=ON
	cmake --build $(BUILD_DIR)/glfw --target=glfw --parallel
	cmake --install $(BUILD_DIR)/glfw --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_jpeg: 
	cd libjpeg-turbo && cmake -S . -B $(BUILD_DIR)/libjpeg-turbo $(CMAKE_FLAGS) -DENABLE_SHARED=NO -DWITH_TOOLS=NO
	cmake --build $(BUILD_DIR)/libjpeg-turbo --parallel
	cmake --install $(BUILD_DIR)/libjpeg-turbo --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_png: build_zlib
	cd libpng && cmake -S . -B $(BUILD_DIR)/png $(CMAKE_FLAGS) -DZLIB_LIBRARY=$(FINAL_DEST_DIR)/lib/libzlib.a -DZLIB_INCLUDE_DIR=$(FINAL_DEST_DIR)/include -DPNG_SHARED=OFF -DPNG_STATIC=ON -DCMAKE_VERBOSE_MAKEFILE=ON
	cmake --build $(BUILD_DIR)/png --target=png_static --parallel
	cmake --install $(BUILD_DIR)/png --prefix $(DEST_DIR)
	# TODO ln -S
	cp $(DEST_DIR)/lib/libpng.a $(DEST_DIR)/lib/libpng16.a
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_zlib:
	cd zlib && cmake -S . -B $(BUILD_DIR)/zlib $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DZLIB_SKIP_SHARED=ON
	cmake --build $(BUILD_DIR)/zlib --target=zlibstatic --parallel
	cmake --install $(BUILD_DIR)/zlib --prefix $(DEST_DIR)
	# TODO ln -S
	cp $(DEST_DIR)/lib/libzlibstatic.a $(DEST_DIR)/lib/libzlib.a
	cp $(DEST_DIR)/lib/libzlibstatic.a $(DEST_DIR)/lib/libz.a
	mkdir -p $(DEST_DIR)/lib/pkgconfig
	cp $(BUILD_DIR)/zlib/zlib.pc $(DEST_DIR)/lib/pkgconfig/libz.pc
	cp $(BUILD_DIR)/zlib/zlib.pc $(DEST_DIR)/lib/pkgconfig/zlib.pc
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_libsamplerate: 
	cd libsamplerate && cmake -S . -B $(BUILD_DIR)/libsamplerate $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/libsamplerate --target=samplerate --parallel
	cmake --install $(BUILD_DIR)/libsamplerate --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_vorbis: build_ogg
	cd vorbis && cmake -S . -B $(BUILD_DIR)/vorbis $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DOGG_LIBRARY=$(FINAL_DEST_DIR)/lib/libogg.a -DOGG_INCLUDE_DIR=$(FINAL_DEST_DIR)/include/ogg/ogg.h -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/vorbis --target=vorbis --parallel
	cmake --build $(BUILD_DIR)/vorbis --target=vorbisenc --parallel
	cmake --build $(BUILD_DIR)/vorbis --target=vorbisfile --parallel
	cmake --install $(BUILD_DIR)/vorbis --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_ogg:
	cd ogg && cmake -S . -B $(BUILD_DIR)/ogg $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/ogg --target=install --parallel
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_freetype: build_zlib build_png
	cd freetype && cmake -S . -B $(BUILD_DIR)/freetype $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DFT_REQUIRE_PNG=ON -DFT_REQUIRE_ZLIB=ON -DPNG_LIBRARY=$(FINAL_DEST_DIR)/lib/libpng.a -DPNG_PNG_INCLUDE_DIR=$(FINAL_DEST_DIR)/include/libpng16 -DZLIB_LIBRARY=$(FINAL_DEST_DIR)/lib/libzlib.a -DZLIB_INCLUDE_DIR=$(FINAL_DEST_DIR)/include -DDISABLE_FORCE_DEBUG_POSTFIX=ON
	cmake --build $(BUILD_DIR)/freetype --target=install --parallel
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_harfbuzz: build_freetype build_glib
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(FINAL_DEST_DIR)/lib/pkgconfig \
	meson setup --reconfigure $(BUILD_DIR)/harfbuzz harfbuzz \
	 	-Dprefix=$(DEST_DIR) \
		-Dglib=enabled \
		-Dutilities=disabled \
		-Dgobject=disabled \
		-Dtests=disabled \
		-Dcairo=disabled \
		$(MESA_FLAG)
	meson install -C $(BUILD_DIR)/harfbuzz
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_curl: build_mbedtls
	cd curl && cmake -S . -B $(BUILD_DIR)/curl $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DCURL_USE_MBEDTLS=ON -DMBEDTLS_INCLUDE_DIR=$(FINAL_DEST_DIR)/include/mbedtls -DMBEDTLS_LIBRARY=$(FINAL_DEST_DIR)/lib/libmbedtls.a -DMBEDX509_LIBRARY=$(FINAL_DEST_DIR)/lib/libmbedx509.a -DMBEDCRYPTO_LIBRARY=$(FINAL_DEST_DIR)/lib/libmbedcrypto.a -DCURL_USE_LIBPSL=OFF -DBUILD_LIBCURL_DOCS=OFF -DBUILD_MISC_DOCS=OFF -DENABLE_CURL_MANUAL=OFF -DBUILD_CURL_EXE=OFF -DENABLE_IPV6=OFF
	cmake --build $(BUILD_DIR)/curl --target=libcurl_static --parallel
	cmake --install $(BUILD_DIR)/curl --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

mbedtls:
	curl -L https://github.com/Mbed-TLS/mbedtls/releases/download/mbedtls-3.6.5/mbedtls-3.6.5.tar.bz2 | tar -xj
	mv mbedtls-3.6.5 mbedtls
	sed -i 's@//#define MBEDTLS_NO_PLATFORM_ENTROPY@#define MBEDTLS_NO_PLATFORM_ENTROPY@' mbedtls/include/mbedtls/mbedtls_config.h
	sed -i 's@#define MBEDTLS_TIMING_C@//#define MBEDTLS_TIMING_C@' mbedtls/include/mbedtls/mbedtls_config.h
	sed -i 's@#define MBEDTLS_NET_C@//#define MBEDTLS_NET_C@' mbedtls/include/mbedtls/mbedtls_config.h
	sed -i 's@defined(__HAIKU__)@defined(__HAIKU__) || defined(__wasi__)@' mbedtls/library/platform_util.c

build_mbedtls: mbedtls
	cd mbedtls && cmake -S . -B $(BUILD_DIR)/mbedtls $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTING=Off -DGEN_FILES=OFF -DENABLE_PROGRAMS=OFF
	cmake --build $(BUILD_DIR)/mbedtls --target=mbedtls --parallel
	cmake --install $(BUILD_DIR)/mbedtls --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_libffi:
    # Delete Makefile in build directory to rerun autogen and configure scripts
	mkdir -p $(BUILD_DIR)/libffi
	[ -f $(BUILD_DIR)/libffi/Makefile ] || (cd libffi && ./autogen.sh && cd $(BUILD_DIR)/libffi && $(CURRENT_DIR)/libffi/configure --host=wasm32-webrogue-wasip3 CC=/wasi-sdk/bin/wasm32-wasip3-clang --prefix=$(DEST_DIR) --disable-shared --enable-static)
	$(MAKE) -C $(BUILD_DIR)/libffi
	$(MAKE) -C $(BUILD_DIR)/libffi install
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_mesa: build_vulkan_headers build_libwr4c
	meson setup --reconfigure $(BUILD_DIR)/mesa mesa \
	 	-Dprefix=$(DEST_DIR) \
		-Dgallium-drivers= \
		-Dvulkan-drivers=virtio \
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
		$(MESA_FLAG)
	ninja -C $(BUILD_DIR)/mesa install
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_vulkan_headers:
	cmake -S Vulkan-Headers -B $(BUILD_DIR)/vulkan_headers $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR)
	cmake --install $(BUILD_DIR)/vulkan_headers --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_angle: build_vulkan_headers
	CONFIG=$(CONFIG) BUILD_DIR=$(BUILD_DIR) sh build_angle.sh
	mkdir -p $(DEST_DIR)/lib
	cp $(BUILD_DIR)/angle/libEGL.a $(DEST_DIR)/lib/libEGL.a
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_glib: build_libffi build_zlib
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(FINAL_DEST_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(FINAL_DEST_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/glib glib \
	 	-Dprefix=$(DEST_DIR) \
		--wrap-mode=nofallback \
		--force-fallback-for=libpcre2-8,intl \
		-Dxattr=false \
		-Dproxy-libintl:default_library=static \
		-Dtests=false \
		$(MESA_FLAG)
	meson install -C $(BUILD_DIR)/glib
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_cairo: build_glib build_png build_fontconfig build_harfbuzz build_freetype build_gdk-pixbuf
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(FINAL_DEST_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(FINAL_DEST_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/cairo cairo \
	 	-Dprefix=$(DEST_DIR) \
		--wrap-mode=nofallback \
		--force-fallback-for=pixman \
		-Dtests=disabled \
		-Dfontconfig=enabled \
		-Dglib=enabled \
		-Dpixman:tests=disabled \
 		-Dpixman:demos=disabled \
		$(MESA_FLAG)

	meson install -C $(BUILD_DIR)/cairo
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_pango: build_cairo build_glib
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(FINAL_DEST_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(FINAL_DEST_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/pango pango \
	 	-Dprefix=$(DEST_DIR) \
		--wrap-mode=nofallback \
		--force-fallback-for=fribidi \
		-Dbuild-utils=false \
		-Dbuild-testsuite=false \
		-Dfribidi:bin=false \
		-Dfribidi:tests=false \
		$(MESA_FLAG)

	meson install -C $(BUILD_DIR)/pango
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_gtk: build_pango build_cairo build_glib build_mesa copy_egl copy_khr build_libxml2 build_gdk-pixbuf build_volk_in_loaders_cloth build_libwr4c
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(FINAL_DEST_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(FINAL_DEST_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/gtk gtk \
	 	-Dprefix=$(DEST_DIR) \
		--wrap-mode=nofallback \
		--force-fallback-for=libepoxy,libjpeg-turbo,librsvg-2.0,libcroco-0.6,libtiff-4,graphene-gobject-1.0 \
		-Dwebrogue-backend=true \
		-Dvulkan=enabled \
		-Dlibepoxy:glx=no \
		-Dlibepoxy:x11=false \
		-Dmedia-gstreamer=disabled \
		-Dbuild-demos=false \
		-Dbuild-tests=false \
		-Dbuild-testsuite=false \
		-Dbuild-examples=false \
		-Dlibjpeg-turbo:tests=disabled \
		-Dlibrsvg:tests=disabled \
		--cross-file=meson_overrides.txt \
		$(MESA_FLAG)
	meson install -C $(BUILD_DIR)/gtk
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_fontconfig: build_expat build_freetype
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(FINAL_DEST_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(FINAL_DEST_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/fontconfig fontconfig \
	 	-Dprefix=$(DEST_DIR) \
		--wrap-mode=nofallback \
		-Dtests=disabled \
		-Dtools=disabled \
		$(MESA_FLAG)
	meson install -C $(BUILD_DIR)/fontconfig
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_libxml2:
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(FINAL_DEST_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(FINAL_DEST_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/libxml2 libxml2 \
	 	-Dprefix=$(DEST_DIR) \
		--wrap-mode=nofallback \
		$(MESA_FLAG)
	meson install -C $(BUILD_DIR)/libxml2
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_gdk-pixbuf: build_libxml2 build_jpeg
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(FINAL_DEST_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(FINAL_DEST_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/gdk-pixbuf gdk-pixbuf \
	 	-Dprefix=$(DEST_DIR) \
		-Djpeg=enabled \
		-Dpng=enabled \
		-Dgif=disabled \
		-Dtiff=disabled \
		-Dman=false \
		-Dgio_sniffing=false \
		-Dbuiltin_loaders=all \
		$(MESA_FLAG)
	meson install -C $(BUILD_DIR)/gdk-pixbuf
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_expat:
	cd libexpat/expat && cmake -S . -B $(BUILD_DIR)/expat $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DEXPAT_BUILD_TESTS=OFF -DEXPAT_BUILD_TOOLS=OFF -DEXPAT_BUILD_EXAMPLES=OFF -DCMAKE_C_FLAGS="-DXML_POOR_ENTROPY"
	cmake --build $(BUILD_DIR)/expat --parallel
	cmake --install $(BUILD_DIR)/expat --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_volk_in_loaders_cloth: build_vulkan_headers
	cd volk-in-loaders-cloth && cmake -S . -B $(BUILD_DIR)/volk-in-loaders-cloth $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DVOLK_IN_LOADERS_CLOTH=ON -DVOLK_INSTALL=ON -DVILC_DEFINES="-DVK_USE_PLATFORM_WEBROGUE"
	cmake --build $(BUILD_DIR)/volk-in-loaders-cloth --target=vulkan --parallel
	cmake --install $(BUILD_DIR)/volk-in-loaders-cloth --prefix $(DEST_DIR)
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

build_shaderc:
	cd shaderc && ./utils/git-sync-deps
	python3 -c 'from pathlib import Path; import re; p=Path("shaderc/third_party/spirv-tools/source/CMakeLists.txt"); s=p.read_text(); marker="# Webrogue: skip the SPIRV-Tools shared target"; pattern=r"# Always build.*?target_compile_definitions\(.*?PUBLIC SPIRV_TOOLS_SHAREDLIB\n\)"; patched=re.sub(pattern, marker, s, count=1, flags=re.S); old_static="set(SPIRV_TOOLS_TARGETS $${SPIRV_TOOLS}-static $${SPIRV_TOOLS}-shared)"; old_default="set(SPIRV_TOOLS_TARGETS $${SPIRV_TOOLS} $${SPIRV_TOOLS}-shared)"; assert marker in s or (patched != s and old_static in s and old_default in s); p.write_text(s if marker in s else patched.replace(old_static, "set(SPIRV_TOOLS_TARGETS $${SPIRV_TOOLS}-static)").replace(old_default, "set(SPIRV_TOOLS_TARGETS $${SPIRV_TOOLS})"))'
	python3 -c 'from pathlib import Path; import re; p=Path("shaderc/libshaderc/CMakeLists.txt"); s=p.read_text(); marker="# Webrogue: shared shaderc target disabled"; patched=re.sub(r"add_library\(shaderc_shared SHARED.*?set_target_properties\(shaderc_shared PROPERTIES SOVERSION 1\)\n", marker+"\n", s, count=1, flags=re.S); patched=re.sub(r"shaderc_add_tests\(\n  TEST_PREFIX shaderc_shared\n.*?^[^\n]*\)\n", "", patched, count=1, flags=re.S|re.M); patched=patched.replace("install(TARGETS shaderc shaderc_shared", "install(TARGETS shaderc"); patched=re.sub(r"target_link_libraries\(shaderc_shared PRIVATE[^\n]*\n", "", patched, count=1); assert marker in s or (marker in patched and "shaderc_shared" not in patched); p.write_text(s if marker in s else patched)'
	cd shaderc && cmake -S . -B $(BUILD_DIR)/shaderc $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DEST_DIR) -DBUILD_SHARED_LIBS=OFF -DSHADERC_SKIP_TESTS=ON -DSHADERC_SKIP_EXAMPLES=ON -DSHADERC_SKIP_COPYRIGHT_CHECK=ON -DSHADERC_ENABLE_WERROR=OFF -DSPIRV_TOOLS_LIBRARY_TYPE=STATIC -DSPIRV_SKIP_EXECUTABLES=ON -DENABLE_GLSLANG_BINARIES=OFF -DSHADERC_SKIP_EXECUTABLES=ON
	cmake --build $(BUILD_DIR)/shaderc --target shaderc SPIRV-Tools-reduce SPIRV-Tools-link SPIRV-Tools-lint SPIRV-Tools-diff spirv-tools-pkg-config glslang MachineIndependent GenericCodeGen glslang-default-resource-limits shaderc_combined shaderc-pkg-config shaderc_static-pkg-config shaderc_combined-pkg-config --parallel
	cmake --install $(BUILD_DIR)/shaderc --prefix $(DEST_DIR)
	rm $(DEST_DIR)/lib/pkgconfig/shaderc.pc
	python3 cached_dist_copy.py $(DEST_DIR) $(FINAL_DEST_DIR)

clean_mbedtls:
	rm -rf mbedtls

clean:
	rm -rf $(BUILD_DIR)
