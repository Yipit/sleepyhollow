# -*- coding: utf-8 -*-
import json
import base64
from os.path import join, abspath, dirname

from tornado.web import Application
from tornado.web import RequestHandler
from tornado.web import StaticFileHandler
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop
from multiprocessing import Process

LOCAL_FILE = lambda *path: join(abspath(dirname(__file__)), *path)


class SimpleHandler(RequestHandler):
    def get(self, name):
        self.render('{}.html'.format(name))


class StatusHandler(RequestHandler):
    def get(self, status):
        self.set_status(int(status))
        self.render('status.html', status=status)


class JSONStatusHandler(RequestHandler):
    def get(self, status):
        data = {'success': True, 'status': int(status)}
        for key in self.request.arguments:
            data[key] = self.get_argument(key)

        self.set_status(int(status))
        self.set_header('Content-Type', 'application/json')
        self.write(json.dumps(data))
        self.finish()


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
