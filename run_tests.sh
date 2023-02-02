#!/bin/bash

echo "Assumes ./bin/YadroTest exists"
echo "Redirects all output to dev/null"
echo "Prints + for files, where no errors were found, - otherwise"

for file in ./test_files/*
do
    ./bin/YadroTest $file &>/dev/null

    if [ $? -eq 0 ]
    then
        echo "====Test + " $(basename $file) "===="
    else
        echo "====Test - " $(basename $file) "===="
        ./bin/YadroTest $file 1>/dev/null
    fi
    echo 

done