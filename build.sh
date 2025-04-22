set -ex
cd $(dirname $0)

cd builder
. $(pwd)/common
cd ..

test -d ../external/aemu && test -d ../external/gfxstream && sh libraries/copy_sources.sh

run_in_docker "$(cat _build.sh)"
