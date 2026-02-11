set -ex
cd $(dirname $0)

cd ../builder
. $(pwd)/common
cd ..

run_in_builder_docker 'rm -rf /package/webrogue-sdk; cp -r /package/webrogue-sdk-$(cat /wasi_arch.txt)-linux /package/webrogue-sdk'
docker build -t $examples_builder_image_name package
