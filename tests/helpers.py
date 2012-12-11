# -*- coding: utf-8 -*-
import os
import sleepyhollow
from sure import scenario


def prepare_server(context):
    context.port = int(os.environ['TEST_SERVER_PORT'])

    def route_to(path):
        params = context.port, path.lstrip('/')
        return 'http://127.0.0.1:%d/%s' % params

    context.route_to = route_to


def clear_server(context):
    pass


server_test_case = scenario(prepare_server, clear_server)


def qt_version_check(min_version):
    def wrapper(func):
        if sleepyhollow.QT_VERSION >= min_version:
            return func
        else:
            return lambda: None
    return wrapper
