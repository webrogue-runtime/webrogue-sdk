#!/bin/sh
# Build the guest with the release SDK package (WEBROGUE_SDK_DEBUG=0).
set -eu
DOCKER_USER_FLAGS=${DOCKER_USER_FLAGS:-}

EXAMPLES_DIR=$(cd "$(dirname "$0")" && pwd)
SDK_DIR=$(dirname "$EXAMPLES_DIR")
REPO_ROOT=${WEBROGUE_REPO_ROOT:-$(git -C "$SDK_DIR" rev-parse --show-superproject-working-tree)}

if [ -z "$REPO_ROOT" ] || [ ! -f "$REPO_ROOT/Cargo.toml" ]; then
    echo "Run this script from the Webrogue repository, or set WEBROGUE_REPO_ROOT." >&2
    exit 1
fi

export WEBROGUE_REPO_ROOT="$REPO_ROOT"
cd "$SDK_DIR/builder"
. ./common
cd "$EXAMPLES_DIR"

run_in_examples_builder_docker "export PKG_CONFIG_PATH=/sdk_dir/package/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip3/lib/pkgconfig; export PKG_CONFIG_LIBDIR=/sdk_dir/package/webrogue-sdk/share/webrogue-sysroot/wasm32-wasip3/lib; make -B -C /sdk_dir/tests/fixtures/stdio && make -B -C /sdk_dir/tests/fixtures/persistence && make -B -C /sdk_dir/tests/fixtures/cpu_rendering && make -B -C /sdk_dir/tests/fixtures/resources"
