set -ex
cd $(dirname $0)

cd ../builder
. $(pwd)/common
cd ../examples

run_in_examples_builder_docker "\
	PKG_CONFIG_PATH=/sdk_dir/package/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip1-threads/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=/sdk_dir/package/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip1-threads/lib \
    make -C /sdk_dir/examples -f docker.Makefile $@
"
