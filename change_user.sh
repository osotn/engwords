#!/bin/sh

dir="./words"
target=active_"${1}"

if [ -d "${dir}"/"${target}" ]; then
    echo "Set new user name"
else
    echo "Error user name"
    exit 1
fi

cd "${dir}"
ln -sfn "${target}" active 
cd ..

