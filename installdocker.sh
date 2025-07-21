#!/bin/bash

# Exit on error and print commands
set -eux

# Update and install dependencies
sudo apt-get update && sudo apt-get install -y --fix-missing \
    git \
    lsb-release \
    python3 \
    python3-pip \
    autoconf \
    bc \
    bison \
    dos2unix \
    gdb \
    gcc \
    lcov \
    make \
    flex \
    build-essential \
    ca-certificates \
    curl \
    device-tree-compiler \
    lcov \
    nano \
    valgrind \
    clang \
    bear

# Install RISC-V Toolchain
WORKDIR=/tmp/riscv-setup
mkdir -p $WORKDIR && cd $WORKDIR

# Detect architecture and download the correct RISC-V toolchain
arch=$(dpkg --print-architecture)
case "$arch" in
    'arm64')
        curl -L -o riscv-gnu-toolchain.tar.gz "https://github.com/langproc/langproc-2022-cw/releases/download/v1.0.0/riscv-gnu-toolchain-2022-09-21-ubuntu-22.04-arm64.tar.gz"
        ;;
    *)
        curl -L -o riscv-gnu-toolchain.tar.gz "https://github.com/langproc/langproc-2022-cw/releases/download/v1.0.0/riscv-gnu-toolchain-2022-09-21-ubuntu-22.04-amd64.tar.gz"
        ;;
esac

# Extract and set PATH
sudo rm -rf /opt/riscv
sudo mkdir -p /opt/riscv
sudo tar -xzf riscv-gnu-toolchain.tar.gz --directory /opt
export PATH="/opt/riscv/bin:$PATH"
export RISCV="/opt/riscv"
echo 'export PATH="/opt/riscv/bin:$PATH"' >> ~/.bashrc
echo 'export RISCV="/opt/riscv"' >> ~/.bashrc

# Test RISC-V GCC
riscv64-unknown-elf-gcc --help

# Install Spike RISC-V ISA Simulator
cd $WORKDIR
git clone https://github.com/riscv-software-src/riscv-isa-sim.git
cd riscv-isa-sim
git checkout v1.1.0
mkdir build
cd build
../configure --prefix=$RISCV --with-isa=RV32IMFD --with-target=riscv32-unknown-elf
make -j$(nproc)
sudo make install
cd $WORKDIR
rm -rf riscv-isa-sim
spike --help

# Install riscv-pk (proxy kernel)
cd $WORKDIR
git clone https://github.com/riscv-software-src/riscv-pk.git
cd riscv-pk
git checkout 573c858d9071a2216537f71de651a814f76ee76d
mkdir build
cd build
../configure --prefix=$RISCV --host=riscv64-unknown-elf --with-arch=rv32imfd --with-abi=ilp32d
make -j$(nproc)
sudo make install

# Cleanup
cd ~
rm -rf $WORKDIR

echo "RISC-V toolchain, Spike, and riscv-pk installed successfully!"