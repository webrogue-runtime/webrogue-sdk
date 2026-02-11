set -ex
cd $(dirname $0)

cd ../builder
. $(pwd)/common
cd ../examples

run_in_examples_builder_docker "\
	PKG_CONFIG_PATH=/opt/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip1-threads/lib/pkgconfig \
	PKG_CONFIG_LIBDIR=/opt/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip1-threads/lib \
    make -C /examples_host_dir -f docker.Makefile $@
"
