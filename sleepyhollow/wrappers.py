#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import urllib
import urlparse

from _sleepyhollow import SleepyHollow as _SleepyHollow
from _sleepyhollow import (
    Error,
    InvalidUrlError,
    ConnectionRefusedError,
)

__all__ = [
    'SleepyHollow',
    'Response',
    'Error',
    'InvalidUrlError',
    'ConnectionRefusedError',
]


class SleepyHollow(_SleepyHollow):
    def request(self, method, url, params=None, headers=None):
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

        try:
            response = super(SleepyHollow, self).request(method, url,
                                                         params=payload,
                                                         headers=headers)
        except KeyboardInterrupt:
            raise SystemExit(1)

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

    def get(self, url, params=None, headers=None):
        url, params = self._patch_querystring(url, params)
        return self.request('get', url=url, params=params, headers=headers)

    def post(self, url, params=None, headers=None):
        return self.request('post', url=url, params=params, headers=headers)

    def put(self, url, params=None, headers=None):
        url, params = self._patch_querystring(url, params)
        return self.request('put', url=url, params=params, headers=headers)

    def head(self, url, params=None, headers=None):
        url, params = self._patch_querystring(url, params)
        return self.request('head', url=url, params=params, headers=headers)

    def delete(self, url, params=None, headers=None):
        url, params = self._patch_querystring(url, params)
        return self.request('delete', url=url, params=params, headers=headers)


class Response(object):
    def __init__(self, status_code, url, text, html, reason, headers,
                 js_errors, requested_resources):
        self.status_code = status_code
        self.url = url
        self.text = unicode(text)
        self.html = unicode(html)
        self.reason = unicode(reason)
        self.content = self.text.encode('utf-8')
        self.headers = headers
        self.js_errors = js_errors
        self.requested_resources = tuple(set(requested_resources))

    @property
    def json(self):
        try:
            return json.loads(self.content)
        except ValueError:
            return None
