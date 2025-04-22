set -ex
cd $(dirname $0)

cd builder
. $(pwd)/common
cd ..

docker build -t webrogue-sdk package
