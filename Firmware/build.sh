#!/bin/bash

# Setup compiler file paths
PATH=$PATH:$(pwd)/compiler/linux/bin

# Setup Project Target
PROJECT_NAME=HelloWorld
ENTITY_NAME=DBG

make -j8 PROJ=$PROJECT_NAME ENTITY=$ENTITY_NAME
