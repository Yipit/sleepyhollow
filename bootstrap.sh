#!/bin/bash
#
# Handy script that cleans up the project folder, re-initializes the
# build system, compiles everything and runs the tests for us.

export DYLD_LIBRARY_PATH=`pwd`/tests
export LD_LIBRARY_PATH=`pwd`/tests

# Cleanup
> .gitignore
git clean -df
git checkout .gitignore

# Compilation
./autogen.sh
make

# Making it easier to run the tests
cp lib/.libs/*.dylib tests
cp python/.libs/*.so tests

# Running the tests
if [ "x$DISPLAY" == "x" ]; then
  xvfb-run --server-args="-screen 0 1024x768x24" nosetests --verbosity=2 -s tests
else
  nosetests --verbosity=2 -s tests
fi

# Cleaning up
rm tests/*.{dylib,so}
