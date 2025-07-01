set -ex
cd $(dirname $0)

cd builder
. $(pwd)/common
cd ..

run_in_docker "$(cat _build.sh)"
