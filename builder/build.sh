. $(dirname $0)/common

sh ../libraries/copy_sources.sh

run_in_docker '
set -ex
make -C /webrogue-sdk/libraries TOOLCHAIN=wasip1 1>/dev/null
cargo build --release --manifest-path=/host_dir/Cargo.toml --no-default-features --features=pack,compile,llvm --target-dir=/host_dir/webrogue-sdk/target
cp /host_dir/webrogue-sdk/target/release/webrogue /opt/webrogue


rsync --recursive /host_dir/aot_artifacts /opt/aot_artifacts

# Android-specific things
# CMDLINE_TOOLS_VERSION=linux-11076708_latest
# export ANDROID_HOME="/opt/Android/Sdk"
# test -d $ANDROID_HOME || mkdir -p $ANDROID_HOME
# test -d $ANDROID_HOME/cmdline-tools || {
#     wget https://dl.google.com/android/repository/commandlinetools-$CMDLINE_TOOLS_VERSION.zip -O $ANDROID_HOME/commandlinetools-$CMDLINE_TOOLS_VERSION.zip
#     unzip $ANDROID_HOME/commandlinetools-$CMDLINE_TOOLS_VERSION.zip -d $ANDROID_HOME
#     rm $ANDROID_HOME/commandlinetools-$CMDLINE_TOOLS_VERSION.zip
# }
# test -d "$ANDROID_HOME/licenses" || {
#     yes | sh $ANDROID_HOME/cmdline-tools/bin/sdkmanager --licenses --sdk_root=$ANDROID_HOME > $ANDROID_HOME/LICENSES.txt
# }

# make -C /opt/simple


# cd /opt
# export GRADLE_USER_HOME=/opt/gradle_cache
# export WEBROGUE_ARTIFACTS_PATH=/opt/aot_artifacts
# /opt/webrogue compile android gradle /opt/simple/simple.wrapp /host_dir/webrogue-sdk/android_aot
'
