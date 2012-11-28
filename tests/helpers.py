# -*- coding: utf-8 -*-
from sure import scenario
from tests.server import Server


def prepare_server(context):
    context.port = 4000
    context.server = Server(context.port)
    context.server.start()

    def route_to(path):
        params = context.port, path.lstrip('/')
        return 'http://127.0.0.1:%d/%s' % params

    context.route_to = route_to


def clear_server(context):
    context.server.stop()


server_test_case = scenario(prepare_server, clear_server)
