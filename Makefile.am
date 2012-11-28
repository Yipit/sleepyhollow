SUBDIRS = include lib python

ACLOCAL_AMFLAGS = -I m4


# Exporting some environment variables to make it easier to run the
# tests
export DYLD_LIBRARY_PATH:=${PWD}/lib/.libs
export LD_LIBRARY_PATH:=${PWD}/lib/.libs
export PYTHONPATH:=${PWD}/python/.libs

test: all
	@nosetests --verbosity=2 -s tests
