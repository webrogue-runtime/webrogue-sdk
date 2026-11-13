FROM ubuntu:26.04

RUN apt-get update \
    && apt-get -y install make wget gpg pkg-config subversion build-essential gcc g++ ninja-build cmake libsdl2-dev libvpx-dev unzip \
    && rm -rf /var/cache/apt/archives /var/lib/apt/lists/*

COPY webrogue-sdk /sdk_dir/package/webrogue-sdk
ENV PATH="/sdk_dir/package/webrogue-sdk/bin/:$PATH"
