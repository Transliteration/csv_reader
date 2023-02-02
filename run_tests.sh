#!/bin/bash

echo 
echo "====  Assumes ./bin/YadroTest exists"
echo "====  Shows only failed test output"
echo "====  Prints + for files, where no errors were found, - otherwise"
echo 

script_path=$( dirname -- "$( readlink -f -- "$0"; )"; )

for file in $script_path/test_files/*
do
    $script_path/bin/YadroTest $file &>/dev/null

    if [ $? -eq 0 ]
    then
        echo "====  Test + " $(basename $file) "===="
    else
        echo "====  Test - " $(basename $file) "===="
        $script_path/bin/YadroTest $file 
    fi
    echo 

done