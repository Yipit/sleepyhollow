SUBDIRS = include lib python

ACLOCAL_AMFLAGS = -I m4


# Exporting some environment variables to make it easier to run the
# tests
export DYLD_LIBRARY_PATH:=${PWD}/lib/.libs
export LD_LIBRARY_PATH:=${PWD}/lib/.libs
export PYTHONPATH:=${PWD}/python/.libs

test: all
	if [ "x$$DISPLAY" == "x" ]; then			\
		xvfb-run --server-args="-screen 0 1024x768x24"	\
			nosetests --verbosity=2 -s tests;	\
	else							\
		nosetests --verbosity=2 -s tests;		\
	fi
