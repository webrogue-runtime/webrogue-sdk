FROM ubuntu

RUN apt-get update
RUN apt-get -y install make wget gpg
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get -y install kitware-archive-keyring
RUN apt-get -y install cmake
RUN rm -rf /var/cache/apt/archives /var/lib/apt/lists/*

COPY webrogue-x86_64-linux /opt/webrogue-sdk
COPY webrogue /opt/webrogue-sdk/bin/webrogue
ENV PATH="/opt/webrogue-sdk/bin/:$PATH"


# COPY --from=builder /wasi-sdk /opt/wasi-sdk
# COPY wasip1 /opt/wasip1
# COPY webrogue /usr/bin/webrogue
# COPY aot_artifacts /opt/aot_artifacts
# ENV WEBROGUE_ARTIFACTS_PATH=/opt/aot_artifacts
# COPY make_symlinks.sh /opt/make_symlinks.sh
# RUN sh /opt/make_symlinks.sh && rm /opt/make_symlinks.sh

# Android-specific things
# RUN apt-get update
# RUN apt-get -y install default-jre
# RUN rm -rf /var/cache/apt/archives /var/lib/apt/lists/*
# COPY Android /opt/Android
# COPY gradle_cache /opt/gradle_cache
# RUN chmod -R a+rwx /opt/gradle_cache
# ENV GRADLE_USER_HOME=/opt/gradle_cache
# ENV ANDROID_HOME=/opt/Android/Sdk
