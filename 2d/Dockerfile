FROM ubuntu:20.04

ENV PATH=/usr/local/bin:/opt/crashsdk/bin:${PATH}
ENV ROOT=/etc/n64
ENV N64_LIBGCCDIR=/opt/crashsdk/lib/gcc/mips64-elf/11.1.0

WORKDIR /usr/local

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get -y install build-essential git wget libmpfr-dev libmpc-dev libgmp-dev flex bison && \
    apt-get clean

RUN dpkg --add-architecture i386 && \
    echo "deb [trusted=yes] https://crashoveride95.github.io/apt/ ./" > /etc/apt/sources.list.d/crashoveride95.list && \
    apt update && \
    apt-get -y install binutils-mips-n64 gcc-mips-n64 libgcc-mips-n64 && \
    apt-get -y install n64sdk && \
    apt-get -y install makemask && \
    apt-get -y install root-compatibility-environment && \
    apt-get -y install libnustd && \
    apt-get -y install n64graphics && \
    apt-get -y install nusys-demos

WORKDIR /n64