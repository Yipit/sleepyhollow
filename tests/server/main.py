# -*- coding: utf-8 -*-
import os
import traceback
import hashlib
import json
import base64
from os.path import join, abspath, dirname, exists

from tornado.web import Application
from tornado.web import RequestHandler
from tornado.web import StaticFileHandler
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop

LOCAL_FILE = lambda *path: join(abspath(dirname(__file__)), *path)


class Session(object):
    # I/O intensive session manager
    SESSION_COOKIE_NAME = 'sleepyhollow-test-cookie-id'
    __new_id = None

    def __init__(self, handler):
        self.handler = handler
        self.data = {}

    def set(self, key, value):
        self.data[key] = value
        self.save()

    def get(self, key, default=None):
        self.load()
        return self.data.get(key, default)

    def pop(self, key, default=None):
        self.load()
        r = self.data.pop(key, default)
        self.save()
        return r

    def save(self):
        with open(self.filename, 'w') as f:
            f.write(json.dumps(self.data))

        self.handler.set_secure_cookie(self.SESSION_COOKIE_NAME, self.id)

    def load(self):
        if not exists(self.filename):
            return

        with open(self.filename, 'r') as f:
            self.data.update(json.loads(f.read()))

    def __setitem__(self, key, value):
        return self.set(key, value)

    def __getitem__(self, key):
        return self.get(key)

    def __delitem__(self, key):
        return self.pop(key)

    def get_new_id(self):
        sha1 = hashlib.sha1()
        sha1.update(os.urandom(128))
        self.__new_id = sha1.hexdigest()
        return self.__new_id

    @property
    def id(self):
        return self.handler.get_secure_cookie(self.SESSION_COOKIE_NAME) or self.new_id

    @property
    def new_id(self):
        return self.__new_id or self.get_new_id()

    @property
    def filename(self):
        return LOCAL_FILE('session-{}.json'.format(self.id))


class SessionHandler(RequestHandler):
    @property
    def session(self):
        return Session(self)


class SimpleHandler(SessionHandler):
    def get(self, name):
        data = {}
        self.session['name'] = name
        for key in self.request.arguments:
            data[key] = self.get_argument(key)

        try:
            self.render('{}.html'.format(name), **data)
        except IOError as e:
            self.render('_error.html', exception=e, tb=traceback.format_exc(e))


class StatusHandler(SessionHandler):
    def get(self, status):
        self.set_status(int(status))
        self.render('status.html', status=status)


class AdminHandler(SessionHandler):
    def get(self, path):
        username = self.session.get('username')

        if not username:
            return self.redirect('/login')

        self.render('admin.html', username=username)


class LoginHandler(SessionHandler):
    def get(self):
        if self.session.get('username'):
            return self.redirect('/admin')

        self.render('login.html', error=None)

    def post(self):
        email = self.get_argument('email')
        if '@' not in email:
            error = 'missing "@" in your email'
            return self.render('login.html', error=error)

        username = email.split('@', 1)[0]
        self.session['username'] = username
        return self.redirect('/admin')


class JSONStatusHandler(SessionHandler):
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


class AuthHandler(SessionHandler):
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


class IndexHandler(SessionHandler):
    def get(self):
        self.render('index.html', routes=Server.routes())


class Server(object):
    def __init__(self, port):
        self.port = int(port)
        self.process = None
        self.options = {
            'template_path': LOCAL_FILE('templates'),
            'cookie_secret': 'cookie-monster',
        }

    @classmethod
    def routes(cls):
        return [
            (r"/status-(\d+)\.json$", JSONStatusHandler),
            (r"/status-(\d+)$", StatusHandler),
            (r"/login", LoginHandler),
            (r"/admin/?(.*)", AdminHandler),
            (r"/(\w+)", SimpleHandler),
            (r"/auth/(\w+)", AuthHandler),
            (r"/media/(.*)", StaticFileHandler, {"path": LOCAL_FILE('media')}),
            (r"/$", IndexHandler),
        ]

    @classmethod
    def get_handlers(cls, options):
        return Application(cls.routes(), **options)

    def start(self):
        app = self.get_handlers(self.options)
        http = HTTPServer(app)
        http.listen(int(self.port))
        IOLoop.instance().start()

    def stop(self):
        self.process.terminate()
