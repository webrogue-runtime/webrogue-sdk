FROM ubuntu

RUN apt-get update
RUN apt-get -y install make wget gpg
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get -y install kitware-archive-keyring
RUN apt-get -y install cmake
RUN apt-get -y install subversion
RUN rm -rf /var/cache/apt/archives /var/lib/apt/lists/*

COPY webrogue-sdk /opt/webrogue-sdk
ENV PATH="/opt/webrogue-sdk/bin/:$PATH"
