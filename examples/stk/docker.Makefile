BUILD_TYPE=Release

stk: stk-assets.timestamp
	cmake -S stk-code -B stk-code/build --toolchain=/opt/webrogue-sdk/share/cmake/wasi-sdk-p1-pthread.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DUSE_GLES2=ON -DUSE_MOJOAL=ON -DNO_SHADERC=ON -DUSE_WIIUSE=OFF -DUSE_CRYPTO_OPENSSL=OFF -DJPEG_INCLUDE_DIR=$SYSROOT_DIR/include/jpeg
	cmake --build stk-code/build --target=supertuxkart --parallel=32

stk-assets.timestamp:
	bash -c "(cd stk-assets && svn cleanup) || true"
	svn co https://svn.code.sf.net/p/supertuxkart/code/stk-assets stk-assets
	cp -r stk-assets/karts stk-code/data/karts
	cp -r stk-assets/library stk-code/data/library
	cp -r stk-assets/models stk-code/data/models
	cp -r stk-assets/music stk-code/data/music
	cp -r stk-assets/sfx stk-code/data/sfx
	cp -r stk-assets/textures stk-code/data/textures
	cp -r stk-assets/tracks stk-code/data/tracks
	touch stk-assets.timestamp

clean:
	rm -rf stk-code/build
