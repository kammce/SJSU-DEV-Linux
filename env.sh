#!/bin/bash
# Setup a base directory:
BASE=/Users/kmwetles/SJSU-DEV-Linux

# SJSUOne Board Settings:
SJSUONEDEV=/dev/tty.usbserial-A503JOLS # Kai's board
#SJSUONEDEV=/dev/tty.usbserial-A503JOL7 # Tom's board
#SJSUONEDEV=/dev/tty.usbserial-A503JOBA # Josh's board
#SJSUONEDEV=/dev/tty.usbserial-A503JOBG # Charles's board
#SJSUONEDEV=/dev/tty.usbserial-A8020B99 # Aaron's board
SJSUONEBAUD=38400

# Project Target Settings:
# Sets the binary name, defaults to "firmware" (Optional)
PROJ=firmware
# Sets which DBC to generate, defaults to "DBG"
ENTITY=DBG

# Compiler and library settings:
# Selects compiler version to use
CC=gcc-arm-none-eabi-5_4-2016q3
# Generates a system-arch string (f.eks.: Linux-x86_64)
OS=`uname -sm | tr ' ' '-'` 
PATH=$PATH:$BASE/util/$CC-$OS/bin:$BASE/util/Hyperload/bin
LIB_DIR="$BASE/lib"

# Make system settings:
# Number of jobs = 8
# Tune this to nthreads (f.eks. my system has a quad core with two threads per core = 8 threads)
MAKEFLAGS=" -j 8"

# Export everything to the environment
export PATH
export PROJ
export ENTITY
export LIB_DIR
export SJSUONEDEV
export SJSUONEBAUD
export MAKEFLAGS
