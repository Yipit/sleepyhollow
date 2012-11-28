export PYTHONPATH:= /usr/local/lib/python2.7/site-packages:${PYTHONPATH}:${PWD}


test: module
	@nosetests --verbosity=2 -s tests

module:
	@printf "\033[1;37mBuilding the C++ module... \033[0"
	@(python setup.py build_ext -i) 2>&1>/dev/null
	@echo "\033[1;32mOK\033[0m"

dependencies:
	@printf "Installing dependencies... "
	@(pip install -U -r requirements.pip) 2>&1>/dev/null
	@echo "OK"
