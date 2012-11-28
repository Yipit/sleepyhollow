export PYTHONPATH:= /usr/local/lib/python2.7/site-packages:${PYTHONPATH}:${PWD}

test: module
	nosetests --verbosity=2 -s tests.py

module:
	python setup.py build_ext -i
