# base image
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# install tools
RUN apt-get update && apt-get install -y \
    build-essential g++ make git wget xz-utils \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

# download Bootlin aarch64 toolchain
ENV TOOLCHAIN_URL=https://toolchains.bootlin.com/downloads/releases/toolchains/aarch64/tarballs/aarch64--glibc--stable-2025.08-1.tar.xz
ENV TOOLCHAIN_DIR=/opt/aarch64-toolchain

RUN mkdir -p $TOOLCHAIN_DIR && \
    wget -O /tmp/toolchain.tar.xz $TOOLCHAIN_URL && \
    tar -xJf /tmp/toolchain.tar.xz -C $TOOLCHAIN_DIR --strip-components=1 && \
    rm /tmp/toolchain.tar.xz

ENV PATH=$TOOLCHAIN_DIR/bin:$PATH

# compile x86_64 executable file
RUN make OUTPUT_DIR=output/x86_64

# compile aarch64 executable file
RUN make OUTPUT_DIR=output/aarch64 CXX=aarch64-buildroot-linux-gnu-g++

CMD ["bash"]
