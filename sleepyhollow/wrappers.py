#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import base64
import urllib
import urlparse

from functools import partial

from _sleepyhollow import SleepyHollow as _SleepyHollow
from _sleepyhollow import (
    Error,
    InvalidUrlError,
    ConnectionRefusedError,
    BadCredentialsError,
)

__all__ = [
    'SleepyHollow',
    'Response',
    'Error',
    'InvalidUrlError',
    'ConnectionRefusedError',
    'BadCredentialsError',
]


class SleepyHollow(_SleepyHollow):
    def __init__(self):
        # Can you hear that? It's Mjolnir!!!!
        for method in 'get', 'post', 'put', 'head', 'delete':
            setattr(self, method, partial(self.request, method))

    def evaluate_javascript(self, script):
        if isinstance(script, str):
            script = script.decode('utf-8')

        return super(SleepyHollow, self).evaluate_javascript(script)

    def request(self, method, url, params=None, headers=None,
                auth=(), config=None):

        method = method.lower()

        if method != 'post':
            url, params = self._patch_querystring(url, params)

        if isinstance(params, dict):
            payload = urllib.urlencode(params)
        else:
            payload = params

        if not headers:
            headers = {}

        if not 'Content-Type' in headers:
            headers['Content-Type'] = 'application/x-www-form-urlencoded'

        if not 'User-Agent' in headers:
            headers['User-Agent'] = 'SleepyHollow v0.0.1'

        response = super(SleepyHollow, self).request(method, url,
                                                     params=payload,
                                                     headers=headers,
                                                     auth=auth,
                                                     config=config)

        return Response(**response)

    def _encode_dict_as_qs(self, params):
        items = []
        for key in params:
            value = params[key]
            if isinstance(value, list):
                for v in value:
                    items.append(urllib.urlencode({key: v}))
            else:
                items.append(urllib.urlencode({key: value}))

        return '&'.join(items)

    def _patch_querystring(self, url, params):
        p = urlparse.urlsplit(url)
        _params = urlparse.parse_qs(p.query)

        if isinstance(params, dict):
            _params.update(params)

        qs = self._encode_dict_as_qs(_params)

        parts = (p.scheme, p.netloc, p.path, qs, p.fragment)
        return urlparse.urlunsplit(parts), _params


class Response(object):
    def __init__(self, status_code, url, text, html, reason, headers,
                 js_errors, requested_resources, screenshot_bytes_base64):

        self.status_code = status_code
        self.url = url
        self.text = unicode(text)
        self.html = unicode(html)
        self.reason = unicode(reason)
        self.content = self.text.encode('utf-8')
        self.headers = headers
        self.js_errors = js_errors
        self.requested_resources = requested_resources
        self.screenshot_bytes = base64.decodestring(screenshot_bytes_base64)

    @property
    def json(self):
        try:
            return json.loads(self.content)
        except ValueError:
            return None

    def save_screenshot(self, path):
        if not self.screenshot_bytes:
            raise ValueError(
                "Screenshot should be enabled throught the config dict")

        fd = open(path, 'wb')
        fd.write(self.screenshot_bytes)
        fd.close()
