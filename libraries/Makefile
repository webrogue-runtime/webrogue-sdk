all: build_volk_in_loaders_cloth copy_pkgconfig copy_gl copy_glad copy_gles2 copy_gles3 copy_egl copy_khr build_webroguegfx build_cxxemulatedthrow build_glfw build_vulkan_headers build_jpeg build_png build_zlib build_libsamplerate build_vorbis build_ogg build_freetype build_harfbuzz build_SDL2 build_SDL3 build_SDL2_ttf build_SDL3_ttf build_angle build_mbedtls build_libffi build_cairo build_pango build_gtk build_glib build_fontconfig build_expat build_libxml2 build_gdk-pixbuf # build_curl
CURRENT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SDK_DIR:=$(shell dirname $(CURRENT_DIR))
DESTINATION_DIR=$(SDK_DIR)/opt/$(TOOLCHAIN)
BUILD_DIR=$(CURRENT_DIR)/build/wasip1/$(CONFIG)
CMAKE_FLAGS=--toolchain=$(DESTINATION_DIR)/toolchain.cmake -DWASI_SDK_PREFIX=/wasi-sdk -DCMAKE_BUILD_TYPE=$(CONFIG) -Wno-dev

copy_pkgconfig:
	mkdir -p $(DESTINATION_DIR)/lib/pkgconfig
	cp -r pkgconfig/*.pc $(DESTINATION_DIR)/lib/pkgconfig

# copy headers
copy_gl: $(DESTINATION_DIR)/include/GL

$(DESTINATION_DIR)/include/GL:
	cp -r GL $(DESTINATION_DIR)/include

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

copy_egl: $(DESTINATION_DIR)/include/EGL

$(DESTINATION_DIR)/include/EGL:
	mkdir -p $(DESTINATION_DIR)/include/EGL
	cp -r EGL/ $(DESTINATION_DIR)/include

# build and install libs
build_webroguegfx: copy_gles2 copy_gles3 copy_khr build_mesa
	cmake -S webroguegfx -B $(BUILD_DIR)/webroguegfx $(CMAKE_FLAGS) -DMESA_PATH=$(BUILD_DIR)/mesa 
	cmake --build $(BUILD_DIR)/webroguegfx --target=webroguegfx --parallel
	cmake --install $(BUILD_DIR)/webroguegfx --prefix $(DESTINATION_DIR)
# 	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_cxxemulatedthrow:
	cd cxxemulatedthrow && cmake -S . -B $(BUILD_DIR)/cxxemulatedthrow $(CMAKE_FLAGS)
	cmake --build $(BUILD_DIR)/cxxemulatedthrow --target=cxxemulatedthrow --parallel
	cmake --install $(BUILD_DIR)/cxxemulatedthrow --prefix $(DESTINATION_DIR)

build_SDL2: build_webroguegfx build_libsamplerate
	cmake -S SDL2 -B $(BUILD_DIR)/SDL2 $(CMAKE_FLAGS) -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_CMAKE_DEBUG_POSTFIX=
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2-static --parallel
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2main --parallel
	cmake --build $(BUILD_DIR)/SDL2 --target=SDL2_test --parallel
	cmake --install $(BUILD_DIR)/SDL2 --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_SDL2_ttf: build_SDL2 build_freetype
	cmake -S SDL2_ttf -B $(BUILD_DIR)/SDL2_ttf $(CMAKE_FLAGS) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=OFF -DSDL2_DIR=$(DESTINATION_DIR)/lib/cmake/SDL2 -DFREETYPE_INCLUDE_DIRS=$(DESTINATION_DIR)/include/freetype2 -DSDL2TTF_DEBUG_POSTFIX=
	cmake --build $(BUILD_DIR)/SDL2_ttf --target=SDL2_ttf --parallel
	cmake --install $(BUILD_DIR)/SDL2_ttf --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_SDL3: build_webroguegfx
	cmake -S SDL3 -B $(BUILD_DIR)/SDL3 $(CMAKE_FLAGS) -DSDL_STATIC=ON -DSDL_SHARED=OFF
	cmake --build $(BUILD_DIR)/SDL3 --target=SDL3-static --parallel
	cmake --build $(BUILD_DIR)/SDL3 --target=SDL3_test --parallel
	cmake --install $(BUILD_DIR)/SDL3 --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_SDL3_ttf: build_SDL3 build_freetype
	cmake -S SDL3_ttf -B $(BUILD_DIR)/SDL3_ttf $(CMAKE_FLAGS) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=OFF -DSDL3_DIR=$(DESTINATION_DIR)/lib/cmake/SDL3 -DFREETYPE_INCLUDE_DIRS=$(DESTINATION_DIR)/include/freetype2
	cmake --build $(BUILD_DIR)/SDL3_ttf --target=SDL3_ttf-static --parallel
	cmake --install $(BUILD_DIR)/SDL3_ttf --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_glfw: build_webroguegfx
	cmake -S glfw -B $(BUILD_DIR)/glfw $(CMAKE_FLAGS) -DGLFW_BUILD_EXAMPLES=ON -DGLFW_BUILD_WEBROGUE=ON
	cmake --build $(BUILD_DIR)/glfw --target=glfw --parallel
	cmake --install $(BUILD_DIR)/glfw --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_jpeg: 
	cd libjpeg-turbo && cmake -S . -B $(BUILD_DIR)/libjpeg-turbo $(CMAKE_FLAGS) -DENABLE_SHARED=NO -DWITH_TOOLS=NO
	cmake --build $(BUILD_DIR)/libjpeg-turbo --parallel
	cmake --install $(BUILD_DIR)/libjpeg-turbo --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_png: build_zlib
	cd libpng && cmake -S . -B $(BUILD_DIR)/png $(CMAKE_FLAGS) -DZLIB_LIBRARY=$(DESTINATION_DIR)/lib/libzlib.a -DZLIB_INCLUDE_DIR=$(DESTINATION_DIR)/include -DPNG_SHARED=OFF -DPNG_STATIC=ON
	cmake --build $(BUILD_DIR)/png --target=png_static --parallel
	cmake --install $(BUILD_DIR)/png --prefix $(DESTINATION_DIR)
	# TODO ln -S
	cp $(DESTINATION_DIR)/lib/libpng.a $(DESTINATION_DIR)/lib/libpng16.a
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_zlib:
	cd zlib && cmake -S . -B $(BUILD_DIR)/zlib $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DZLIB_SKIP_SHARED=ON
	cmake --build $(BUILD_DIR)/zlib --target=zlibstatic --parallel
	cmake --install $(BUILD_DIR)/zlib --prefix $(DESTINATION_DIR)
	# TODO ln -S
	cp $(DESTINATION_DIR)/lib/libzlibstatic.a $(DESTINATION_DIR)/lib/libzlib.a
	cp $(DESTINATION_DIR)/lib/libzlibstatic.a $(DESTINATION_DIR)/lib/libz.a
	cp $(BUILD_DIR)/zlib/zlib.pc $(DESTINATION_DIR)/lib/pkgconfig/libz.pc
	cp $(BUILD_DIR)/zlib/zlib.pc $(DESTINATION_DIR)/lib/pkgconfig/zlib.pc
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_libsamplerate: 
	cd libsamplerate && cmake -S . -B $(BUILD_DIR)/libsamplerate $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/libsamplerate --target=samplerate --parallel
	cmake --install $(BUILD_DIR)/libsamplerate --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_vorbis: build_ogg
	cd vorbis && cmake -S . -B $(BUILD_DIR)/vorbis $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DOGG_LIBRARY=$(DESTINATION_DIR)/lib/libogg.a -DOGG_INCLUDE_DIR=$(DESTINATION_DIR)/include/ogg/ogg.h -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/vorbis --target=vorbis --parallel
	cmake --build $(BUILD_DIR)/vorbis --target=vorbisenc --parallel
	cmake --build $(BUILD_DIR)/vorbis --target=vorbisfile --parallel
	cmake --install $(BUILD_DIR)/vorbis --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_ogg:
	cd ogg && cmake -S . -B $(BUILD_DIR)/ogg $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5
	cmake --build $(BUILD_DIR)/ogg --target=install --parallel
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_freetype: build_zlib build_png
	cd freetype && cmake -S . -B $(BUILD_DIR)/freetype $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DFT_REQUIRE_PNG=ON -DFT_REQUIRE_ZLIB=ON -DPNG_LIBRARY=$(DESTINATION_DIR)/lib/libpng.a -DPNG_PNG_INCLUDE_DIR=$(DESTINATION_DIR)/include/libpng16 -DZLIB_LIBRARY=$(DESTINATION_DIR)/lib/libzlib.a -DZLIB_INCLUDE_DIR=$(DESTINATION_DIR)/include -DDISABLE_FORCE_DEBUG_POSTFIX=ON
	cmake --build $(BUILD_DIR)/freetype --target=install --parallel
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_harfbuzz: build_freetype build_glib
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(DESTINATION_DIR)/lib/pkgconfig \
	meson setup --reconfigure $(BUILD_DIR)/harfbuzz harfbuzz \
	 	-Dprefix=$(DESTINATION_DIR) \
		-Ddefault_library=static \
		-Dglib=enabled \
		-Dutilities=disabled \
		-Dgobject=disabled \
		-Dtests=disabled \
		-Dcairo=disabled \
		--buildtype=$(CONFIG_LOWERCASED) \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt
	meson install -C $(BUILD_DIR)/harfbuzz
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_curl: build_mbedtls
	cd curl && cmake -S . -B $(BUILD_DIR)/curl $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DCURL_USE_MBEDTLS=ON -DMBEDTLS_INCLUDE_DIR=$(DESTINATION_DIR)/include/mbedtls -DMBEDTLS_LIBRARY=$(DESTINATION_DIR)/lib/libmbedtls.a -DMBEDX509_LIBRARY=$(DESTINATION_DIR)/lib/libmbedx509.a -DMBEDCRYPTO_LIBRARY=$(DESTINATION_DIR)/lib/libmbedcrypto.a -DCURL_USE_LIBPSL=OFF -DBUILD_LIBCURL_DOCS=OFF -DBUILD_MISC_DOCS=OFF -DENABLE_CURL_MANUAL=OFF -DBUILD_CURL_EXE=OFF -DENABLE_IPV6=OFF
	cmake --build $(BUILD_DIR)/curl --target=libcurl_static --parallel
	cmake --install $(BUILD_DIR)/curl --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

mbedtls:
	curl -L https://github.com/Mbed-TLS/mbedtls/releases/download/mbedtls-3.6.5/mbedtls-3.6.5.tar.bz2 | tar -xj
	mv mbedtls-3.6.5 mbedtls
	sed -i 's@//#define MBEDTLS_NO_PLATFORM_ENTROPY@#define MBEDTLS_NO_PLATFORM_ENTROPY@' mbedtls/include/mbedtls/mbedtls_config.h
	sed -i 's@#define MBEDTLS_TIMING_C@//#define MBEDTLS_TIMING_C@' mbedtls/include/mbedtls/mbedtls_config.h
	sed -i 's@#define MBEDTLS_NET_C@//#define MBEDTLS_NET_C@' mbedtls/include/mbedtls/mbedtls_config.h
	sed -i 's@defined(__HAIKU__)@defined(__HAIKU__) || defined(__wasi__)@' mbedtls/library/platform_util.c

build_mbedtls: mbedtls
	cd mbedtls && cmake -S . -B $(BUILD_DIR)/mbedtls $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTING=Off -DGEN_FILES=OFF -DENABLE_PROGRAMS=OFF
	cmake --build $(BUILD_DIR)/mbedtls --target=mbedtls --parallel
	cmake --install $(BUILD_DIR)/mbedtls --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_libffi:
    # Delete Makefile in build directory to rerun autogen and configure scripts
	mkdir -p $(BUILD_DIR)/libffi
	[ -f $(BUILD_DIR)/libffi/Makefile ] || (cd libffi && ./autogen.sh && cd $(BUILD_DIR)/libffi && $(CURRENT_DIR)/libffi/configure --host=wasm32-webrogue-wasip1 CC=/wasi-sdk/bin/wasm32-wasip1-threads-clang --prefix=$(DESTINATION_DIR) --disable-shared --enable-static)
	$(MAKE) -C $(BUILD_DIR)/libffi
	$(MAKE) -C $(BUILD_DIR)/libffi install
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_mesa: build_vulkan_headers
	meson setup --reconfigure $(BUILD_DIR)/mesa mesa \
	 	-Dprefix=$(DESTINATION_DIR) \
		-Ddefault_library=static \
		--buildtype=$(CONFIG_LOWERCASED) \
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
	ninja -C $(BUILD_DIR)/mesa install

build_vulkan_headers:
	cmake -S Vulkan-Headers -B $(BUILD_DIR)/vulkan_headers $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR)
	cmake --install $(BUILD_DIR)/vulkan_headers --prefix $(DESTINATION_DIR)

build_angle: build_vulkan_headers
	CONFIG=$(CONFIG) BUILD_DIR=$(BUILD_DIR) sh build_angle.sh
	mkdir -p $(DESTINATION_DIR)/lib
	cp $(BUILD_DIR)/angle/libEGL.a $(DESTINATION_DIR)/lib/libEGL.a

build_glib: build_libffi build_zlib
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(DESTINATION_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(DESTINATION_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/glib glib \
	 	-Dprefix=$(DESTINATION_DIR) \
		--wrap-mode=nofallback \
		--force-fallback-for=libpcre2-8,intl \
		-Dxattr=false \
		-Ddefault_library=static \
		-Dproxy-libintl:default_library=static \
		-Dtests=false \
		--buildtype=$(CONFIG_LOWERCASED) \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt
	meson install -C $(BUILD_DIR)/glib
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_cairo: build_glib build_png build_fontconfig build_harfbuzz build_freetype  build_gdk-pixbuf
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(DESTINATION_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(DESTINATION_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/cairo cairo \
	 	-Dprefix=$(DESTINATION_DIR) \
		--wrap-mode=nofallback \
		--force-fallback-for=pixman \
		-Ddefault_library=static \
		-Dtests=disabled \
		-Dfontconfig=enabled \
		-Dglib=enabled \
		-Dpixman:tests=disabled \
 		-Dpixman:demos=disabled \
		--buildtype=$(CONFIG_LOWERCASED) \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt

	meson install -C $(BUILD_DIR)/cairo
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_pango: build_cairo build_glib
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(DESTINATION_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(DESTINATION_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/pango pango \
	 	-Dprefix=$(DESTINATION_DIR) \
		--wrap-mode=nofallback \
		--force-fallback-for=fribidi \
		-Ddefault_library=static \
		-Dbuild-utils=false \
		-Dbuild-testsuite=false \
		-Dfribidi:bin=false \
		-Dfribidi:tests=false \
		--buildtype=$(CONFIG_LOWERCASED) \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt

	meson install -C $(BUILD_DIR)/pango
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_gtk: build_pango build_cairo build_glib build_mesa copy_egl copy_khr build_libxml2 build_gdk-pixbuf build_volk_in_loaders_cloth build_webroguegfx
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(DESTINATION_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(DESTINATION_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/gtk gtk \
	 	-Dprefix=$(DESTINATION_DIR) \
		--wrap-mode=nofallback \
		--force-fallback-for=libepoxy,libjpeg-turbo,librsvg-2.0,libcroco-0.6,libtiff-4,graphene-gobject-1.0 \
		-Dwebrogue-backend=true \
		-Dvulkan=enabled \
		-Dlibepoxy:glx=no \
		-Dlibepoxy:x11=false \
		-Dmedia-gstreamer=disabled \
		-Ddefault_library=static \
		-Dbuild-demos=false \
		-Dbuild-tests=false \
		-Dbuild-testsuite=false \
		-Dbuild-examples=false \
		-Dlibjpeg-turbo:tests=disabled \
		-Dlibrsvg:tests=disabled \
		--buildtype=$(CONFIG_LOWERCASED) \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt \
		--cross-file=meson_overrides.txt
	meson install -C $(BUILD_DIR)/gtk
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_fontconfig: build_expat build_freetype
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(DESTINATION_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(DESTINATION_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/fontconfig fontconfig \
	 	-Dprefix=$(DESTINATION_DIR) \
		--wrap-mode=nofallback \
		-Ddefault_library=static \
		-Dtests=disabled \
		-Dtools=disabled \
		--buildtype=$(CONFIG_LOWERCASED) \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt
	meson install -C $(BUILD_DIR)/fontconfig
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_libxml2:
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(DESTINATION_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(DESTINATION_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/libxml2 libxml2 \
	 	-Dprefix=$(DESTINATION_DIR) \
		--wrap-mode=nofallback \
		-Ddefault_library=static \
		--buildtype=$(CONFIG_LOWERCASED) \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt
	meson install -C $(BUILD_DIR)/libxml2
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_gdk-pixbuf: build_libxml2 build_jpeg
	PKG_CONFIG=/usr/bin/pkg-config \
	PKG_CONFIG_PATH=$(DESTINATION_DIR)/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=$(DESTINATION_DIR)/lib \
	meson setup --reconfigure $(BUILD_DIR)/gdk-pixbuf gdk-pixbuf \
	 	-Dprefix=$(DESTINATION_DIR) \
		-Ddefault_library=static \
		-Djpeg=enabled \
		-Dpng=enabled \
		-Dgif=disabled \
		-Dtiff=disabled \
		-Dman=false \
		-Dgio_sniffing=false \
		-Dbuiltin_loaders=all \
		--buildtype=$(CONFIG_LOWERCASED) \
		--cross-file=$(DESTINATION_DIR)/meson_cross.txt
	meson install -C $(BUILD_DIR)/gdk-pixbuf
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_expat:
	cd libexpat/expat && cmake -S . -B $(BUILD_DIR)/expat $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DEXPAT_BUILD_TESTS=OFF -DEXPAT_BUILD_TOOLS=OFF -DEXPAT_BUILD_EXAMPLES=OFF -DCMAKE_C_FLAGS="-DXML_POOR_ENTROPY"
	cmake --build $(BUILD_DIR)/expat --parallel
	cmake --install $(BUILD_DIR)/expat --prefix $(DESTINATION_DIR)
	python3 fix_pkgconfig_prefix.py $(DESTINATION_DIR)/lib/pkgconfig/*.pc

build_volk_in_loaders_cloth: build_vulkan_headers
	cd volk-in-loaders-cloth && cmake -S . -B $(BUILD_DIR)/volk-in-loaders-cloth $(CMAKE_FLAGS) -DCMAKE_INSTALL_PREFIX=$(DESTINATION_DIR) -DBUILD_SHARED_LIBS=OFF -DVOLK_IN_LOADERS_CLOTH=ON -DVOLK_INSTALL=ON -DVILC_DEFINES="-DVK_USE_PLATFORM_WEBROGUE"
	cmake --build $(BUILD_DIR)/volk-in-loaders-cloth --target=vulkan --parallel
	cmake --install $(BUILD_DIR)/volk-in-loaders-cloth --prefix $(DESTINATION_DIR)

clean_mbedtls:
	rm -rf mbedtls

clean:
	rm -rf $(BUILD_DIR)
