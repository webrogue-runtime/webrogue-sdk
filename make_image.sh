set -ex
cd $(dirname $0)

cd builder
. $(pwd)/common
cd ..

run_in_docker '
set -ex
cargo build --release --manifest-path=/host_dir/Cargo.toml --no-default-features --features=pack --target-dir=/host_dir/webrogue-sdk/target
cp /host_dir/webrogue-sdk/target/release/webrogue /opt/webrogue
'
cp opt/webrogue package/webrogue

docker build -t webrogue-sdk package
