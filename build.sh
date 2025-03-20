set -ex
cd $(dirname $0)

sh builder/build.sh

docker build -t webrogue-sdk opt
