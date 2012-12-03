# -*- coding: utf-8 -*-
import sys
import time
from tests.server.main import Server


if __name__ == '__main__':
    port = 4000
    if sys.argv[-1].isdigit():
        port = int(sys.argv[-1])

    server = Server(port)
    server.start()
    started = time.time()
    print "The server is running on port %d...\n" % port
    while True:
        try:
            time.sleep(1)
            print "\033[Auptime: %d" % (time.time() - started)
        except KeyboardInterrupt:
            print "\rControl-C was pressed\n"
            raise SystemExit(1)
