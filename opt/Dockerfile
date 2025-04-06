FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get -y install curl
RUN curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-25/wasi-sdk-25.0-x86_64-linux.tar.gz | tar -xz && mv wasi-sdk-25.0-x86_64-linux /wasi-sdk


FROM ubuntu:24.04

RUN apt-get update
RUN apt-get -y install make wget gpg
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get -y install kitware-archive-keyring
RUN apt-get -y install cmake
RUN rm -rf /var/cache/apt/archives /var/lib/apt/lists/*

COPY --from=builder /wasi-sdk /opt/wasi-sdk
COPY wasip1 /opt/wasip1
COPY webrogue /usr/bin/webrogue
