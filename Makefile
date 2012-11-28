export PYTHONPATH:= /usr/local/lib/python2.7/site-packages:${PYTHONPATH}:${PWD}


test: module
	@nosetests --verbosity=2 -s tests

module: clear
	@python setup.py build_ext -i

dependencies: clear
	@printf "Installing dependencies... "
	@(pip install -U -r requirements.pip) 2>&1>/dev/null
	@echo "OK"

clear:
	@rm -f sleepy_hollow.so
