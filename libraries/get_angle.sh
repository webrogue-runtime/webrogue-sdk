set -ex
cd $(dirname $0)

rm -rf angle chromium-build

[ -d "depot_tools" ] || git clone --single-branch --depth=1 https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH="$(pwd)/depot_tools:$PATH"

export VPYTHON_VIRTUALENV_ROOT=/tmp/vpython-root
export XDG_CACHE_HOME=/tmp
export HOME=/tmp

git clone https://github.com/webrogue-runtime/angle.git -b webrogue
gclient sync --no-history --shallow

git clone https://github.com/webrogue-runtime/chromium-build.git -b webrogue
cp angle/build/config/gclient_args.gni chromium-build/config/gclient_args.gni
cd angle
rm -rf build
ln -sf ../chromium-build build
cd ..
