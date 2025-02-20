#!/bin/bash

TESTS_DIR="/home/vvdovin/aqua/tests/riscv-qemu/uboot-tests"
# &

coproc QEMU { qemu-system-riscv64 -nographic ; }

echo "RESETTTTTTTTTT $TESTS_DIR"


