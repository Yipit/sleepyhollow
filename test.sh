#!/bin/bash

> .gitignore
git clean -df
./autogen.sh
make
cp lib/.libs/* tests
cp python/.libs/* tests
export DYLD_LIBRARY_PATH=`pwd`/tests
clear
nosetests --verbosity=2 -s tests/test*.py
