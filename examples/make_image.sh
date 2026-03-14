set -ex
cd $(dirname $0)

cd ../builder
. $(pwd)/common
cd ..

run_in_builder_docker 'rm -rf /sdk_dir/package/webrogue-sdk; cp -r /sdk_dir/package/webrogue-sdk-$(cat /wasi_arch.txt)-linux /sdk_dir/package/webrogue-sdk'
docker build -t $examples_builder_image_name package
