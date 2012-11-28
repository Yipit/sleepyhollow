# -*- coding: utf-8 -*-
from sure import scenario, action_for
from tests.server import Server


def prepare_server(context):
    context.port = 4000
    context.server = Server(context.port)
    context.server.start()

    @action_for(context, provides=['route_to'])
    def route_to(path):
        params = context.port, path.lstrip('/')
        return 'http://127.0.0.1:%d/%s' % params


def clear_server(context):
    context.server.stop()


server_test_case = scenario(prepare_server, clear_server)
