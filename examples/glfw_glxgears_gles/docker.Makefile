gears: main.wasm
	echo

CC = "/sdk_dir/package/webrogue-sdk/bin/wasm32-wasip3-clang"

CFLAGS = --target=wasm32-wasip3 \
	-pthread \
	-g \
	-O0 \
	-I/sdk_dir/package/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip3/include \
	-L/sdk_dir/package/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip3/lib

CLFLAGS = -Wl,--no-entry \
	-lglfw3 \
	-lwr4c \
	-lc++ \
	-lc++abi \
	-lEGL \
	-Wl,--export=eglGetProcAddress \
	-lvulkan \
	-lvulkan_virtio \

main.wasm: gears.c docker.Makefile
	${CC} $(CFLAGS) $(CLFLAGS) gears.c \
		-o main.wasm

clean:
	rm -f main.d main.wasm
