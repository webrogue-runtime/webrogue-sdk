simple: simple.wrapp

_BUILD: main.c Makefile
	/wasi-sdk/bin/clang main.c --target=wasm32-wasip1-threads -pthread -g -O0 -Wl,--no-entry -Wl,--import-memory -Wl,--export-memory -Wl,--max-memory=4294967296  -o main.wasm.hot #-nostdlib 

main.wasm: _BUILD
	cmp -s main.wasm.hot main.wasm; \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
	    echo; \
	else \
	    echo; \
	    cp main.wasm.hot main.wasm; \
	fi

simple.wrapp: main.wasm webrogue.json
	rm -f simple.wrapp
	/opt/webrogue pack -o simple.wrapp

clean:
	rm -f main.wasm simple.wrapp
