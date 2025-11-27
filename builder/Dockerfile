FROM ubuntu:25.04

RUN apt-get update && apt-get -y install ca-certificates gpg wget curl build-essential lld llvm bzip2 subversion git unzip rsync python3 default-jre meson python3-mako

RUN bash -c 'case "$(uname -p)" in aarch64) echo arm64 ;; *) uname -p ;; esac' > /wasi_arch.txt
RUN curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-28/wasi-sdk-28.0-$(cat /wasi_arch.txt)-linux.tar.gz | tar -xz && mv wasi-sdk-28.0-$(cat /wasi_arch.txt)-linux /wasi-sdk

RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get -y install kitware-archive-keyring
RUN apt-get -y install cmake

ENV PATH="/fakehome/.cargo/bin:$PATH"
ENV RUSTUP_HOME=/fakehome/rustup
ENV CARGO_HOME=/fakehome/.cargo
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | HOME=/fakehome sh -s -- -y --no-modify-path
# RUN curl -L --proto '=https' --tlsv1.2 -sSf https://raw.githubusercontent.com/cargo-bins/cargo-binstall/main/install-from-binstall-release.sh | bash
# RUN cargo binstall wasm-tools
RUN chmod -R a+rwx /fakehome/.cargo/
# RUN chmod -R a+r /wasi_arch.txt
