#!/bin/bash

script_path=$( dirname -- "$( readlink -f -- "$0"; )"; )

mkdir -p $script_path/build $script_path/bin
cd $script_path/build
cmake ..
make