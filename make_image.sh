set -ex
cd $(dirname $0)

cd builder
. $(pwd)/common
cd ..

run_in_docker 'rm -rf /package/webrogue-sdk; cp -r /package/webrogue-sdk-$(cat /wasi_arch.txt)-linux /package/webrogue-sdk'
docker build --platform linux/amd64 -t webrogue-sdk package
