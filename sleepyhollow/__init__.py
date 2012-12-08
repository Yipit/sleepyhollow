# Just forwarding the symbols declared in the C extension and its pure
# python wrappers
from _sleepyhollow import *
from wrappers import *

# Without this call everything blows up! We need it to create the
# QApplication instance that will be used by our library. We also have a
# function to clean it up, the `teardown()` one, but I'm afraid that we
# won't have the chance to use it in python, because of this:
#
# http://bugs.python.org/issue9072
setup()
