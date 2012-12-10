# -*- coding: utf-8 -*-
import json
import base64
from os.path import join, abspath, dirname

from tornado.web import Application
from tornado.web import RequestHandler
from tornado.web import StaticFileHandler
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop

LOCAL_FILE = lambda *path: join(abspath(dirname(__file__)), *path)


class SimpleHandler(RequestHandler):
    def get(self, name):
        data = {}

        for key in self.request.arguments:
            data[key] = self.get_argument(key)

        self.render('{}.html'.format(name), **data)


class StatusHandler(RequestHandler):
    def get(self, status):
        self.set_status(int(status))
        self.render('status.html', status=status)


class JSONStatusHandler(RequestHandler):
    def make_data(self, method, status):
        # takes method, status code and request params and tuck them
        # into a dict

        data = {'success': True, 'status': int(status), 'method': method}
        for key in self.request.arguments:
            data[key] = self.get_argument(key)

        for key in self.request.headers:
            if key.startswith('X'):
                data[key] = self.request.headers[key]

        return data

    def handle_status(self, method, status):
        # takes a HTTP method + status code and send to make_data(),
        # so its return value will be turned into JSON and returned to
        # the client
        data = self.make_data(method, status)
        self.set_status(int(status))

        self.set_header('Content-Type', 'application/json')
        self.write(json.dumps(data))
        self.finish()

    def handle_header_only_operations(self, method, status):
        # workaround for testing HEAD and DELETE methods.  rather than
        # returning the data from make_data() as json, it returns as
        # X-headers in the response

        self.set_status(int(status))
        data = self.make_data(method, status)
        for key, value in data.items():
            self.set_header('X-%s' % key, json.dumps(value))

        return self.finish()

    def get(self, status):
        return self.handle_status('GET', status)

    def post(self, status):
        return self.handle_status('POST', status)

    def put(self, status):
        return self.handle_status('PUT', status)

    def head(self, status):
        return self.handle_header_only_operations('HEAD', status)

    def delete(self, status):
        return self.handle_header_only_operations('DELETE', status)


class AuthHandler(RequestHandler):
    def ask_for_pw(self):
        self.set_status(401)
        self.set_header('WWW-Authenticate', 'Basic realm=Restricted')
        self._transforms = []
        self.finish()

    def require_auth(self):
        auth_header = self.request.headers.get('Authorization')
        if auth_header:
            auth_decoded = base64.decodestring(auth_header[6:])
            return auth_decoded.split(':', 2)

        self.ask_for_pw()
        return None, None

    def get(self, name):
        user, password = self.require_auth()

        passed = True
        if user != 'lincoln':
            passed = False

        if password != 'gabriel':
            passed = False

        if not passed:
            self.ask_for_pw()

        self.render('{}.html'.format(name))


class Server(object):
    def __init__(self, port):
        self.port = int(port)
        self.process = None
        self.options = {
            'template_path': LOCAL_FILE('templates'),
            'cookie_secret': 'cookie-monster',
        }

    @classmethod
    def get_handlers(cls, options):
        return Application([
            (r"/status-(\d+)\.json$", JSONStatusHandler),
            (r"/status-(\d+)$", StatusHandler),
            (r"/(\w+)", SimpleHandler),
            (r"/auth/(\w+)", AuthHandler),
            (r"/media/(.*)", StaticFileHandler, {"path": LOCAL_FILE('media')}),
        ], **options)

    def start(self):
        app = self.get_handlers(self.options)
        http = HTTPServer(app)
        http.listen(int(self.port))
        IOLoop.instance().start()

    def stop(self):
        self.process.terminate()
