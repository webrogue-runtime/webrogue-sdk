set -ex
cd $(dirname $0)

cd builder
. $(pwd)/common
cd ..

run_in_docker "
    export WEBROGUE_SDK_UNCACHED="$WEBROGUE_SDK_UNCACHED";
    export WEBROGUE_SDK_DEBUG="$WEBROGUE_SDK_DEBUG";
    $(cat _build.sh)
"
