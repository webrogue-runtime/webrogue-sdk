gears: main.wasm
	echo

CC = "/opt/webrogue-sdk/bin/clang"

CFLAGS = --target=wasm32-wasip1-threads \
	-pthread \
	-g \
	-O0 \
	-I/opt/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip1-threads/include \
	-L/opt/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip1-threads/lib

CLFLAGS = -Wl,--no-entry \
	-Wl,--import-memory \
	-Wl,--export-memory \
	-Wl,--max-memory=4294967296 \
	-lglfw3 \
	-lwebroguegfx \
	-lc++ \
	-lc++abi \
	-lEGL \
	-Wl,--export=eglGetProcAddress \
	-lvulkan \
	-lvulkan_gfxstream \

main.wasm: gears.c docker.Makefile
	${CC} $(CFLAGS) $(CLFLAGS) gears.c \
		-o main.wasm

clean:
	rm -f main.d main.wasm
