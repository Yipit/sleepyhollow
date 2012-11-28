#!/bin/sh

echo "Using autotools to generate the build scripts"
autoreconf -i

echo "Configuring the build. It will break if you don't have something that we need"
./configure $@

echo "Now just type make to compile everything"
