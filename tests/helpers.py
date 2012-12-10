# -*- coding: utf-8 -*-
import os
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


def mac_only(func):
    if os.uname()[0] != 'Darwin':
        return lambda: None

    return func
