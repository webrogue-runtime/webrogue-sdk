. $(dirname $0)/common

sh ../libraries/copy_sources.sh

run_in_docker "
make -C /webrogue-sdk/libraries TOOLCHAIN=wasip1 1>/dev/null
test -f /opt/webrogue-wrapp-cli && exit
cargo build --release --manifest-path=/host_dir/crates/wrapp-cli/Cargo.toml --target-dir=/host_dir/webrogue-sdk/target
cp /host_dir/webrogue-sdk/target/release/webrogue-wrapp-cli /opt/webrogue-wrapp-cli
"
