#!/bin/bash
#
# Handy script that cleans up the project folder, re-initializes the
# build system, compiles everything and runs the tests for us.

export DYLD_LIBRARY_PATH=`pwd`/tests

# Cleanup
> .gitignore
git clean -df

# Compilation
./autogen.sh
make

# Making it easier to run the tests
cp lib/.libs/*.{dylib,so} tests
cp python/.libs/*.{dylib,so} tests

# Running the tests
nosetests --verbosity=2 -s tests/test*.py

# Cleaning up
rm tests/*.{dylib,so}
