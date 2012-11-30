#!/bin/bash
export DYLD_LIBRARY_PATH=`pwd`/tests

> .gitignore
git clean -df
./autogen.sh
make
cp lib/.libs/*.{dylib,so} tests
cp python/.libs/*.{dylib,so} tests

nosetests --verbosity=2 -s tests/test*.py
