. $(dirname $0)/common

sh ../libraries/copy_sources.sh

run_in_docker "
make -C /webrogue-sdk/libraries TOOLCHAIN=wasip1 1>/dev/null
cargo build --release --manifest-path=/host_dir/Cargo.toml --no-default-features --features=pack --target-dir=/host_dir/webrogue-sdk/target
cp /host_dir/webrogue-sdk/target/release/webrogue /opt/webrogue
"
