#!/bin/sh

echo "Using autotools to generate the build scripts"
autoreconf -i

if [ "$?" != "0" ]; then
    exit $?
fi;

echo "Configuring the build. It will break if you don't have something that we need"
./configure $@

if [ "$?" != "0" ]; then
    exit $?
fi;

echo "Now just type make to compile everything"
