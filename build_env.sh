#!/bin/bash
export MPP_SRC_DIR=$(realpath `pwd`)
export RTSMART_SRC_DIR=$(realpath ${MPP_SRC_DIR}/../rtsmart)
export SDK_SRC_ROOT_DIR=$(realpath ${RTSMART_SRC_DIR}/../../../)
export RTT_CC=gcc
export RTT_CC_PREFIX=riscv64-unknown-linux-musl-
export RTT_EXEC_PATH=~/.kendryte/k230_toolchains/riscv64-linux-musleabi_for_x86_64-pc-linux-gnu/bin
export PATH=$PATH:$RTT_EXEC_PATH
