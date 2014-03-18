#!/bin/bash

# acquire current _Build Directory
ORGINAL_DIR=$(pwd)
# goto gcc-arm-non-eabi directory
cd ../../../gcc-arm-none-eabi*/bin
# acquire gcc-arm-none-eabi full absolute directory
ARM_DIR=$(pwd)
# Adding gcc-arm-none-eabi directory to PATH
#echo -e "Adding gcc-arm-none-eabi to PATH:\n\t$ARM_DIR"
export PATH=$PATH:$ARM_DIR
# go back to original directory
cd $ORGINAL_DIR
# using Makefile to build the entire project or just what has been modified
make all
echo "Press any key to end..."
read -s -n 1 any_key
exit 0