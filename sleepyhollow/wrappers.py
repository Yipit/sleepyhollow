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
    def request(self, method, url, params=None):
        if isinstance(params, dict):
            payload = urllib.urlencode(params)
        else:
            payload = params

        response = super(SleepyHollow, self).request(method, url, params=payload)

        return Response(
            status_code=response['status_code'],
            url=response['url'],
            text=response['text'],
            reason=response['reason'],
            headers=response['headers'],
        )

    def _patch_querystring(self, url, params):
        p = urlparse.urlsplit(url)
        _params = urlparse.parse_qs(p.query)
        if isinstance(params, dict):
            _params.update(params)

        parts = (p.scheme, p.netloc, p.path, urllib.urlencode(_params), p.fragment)
        return urlparse.urlunsplit(parts)

    def get(self, url, params=None):
        url = self._patch_querystring(url, params)
        return self.request('get', url=url, params=params)

    def post(self, url, params=None):
        return self.request('post', url=url, params=params)

    def put(self, url, params=None):
        url = self._patch_querystring(url, params)
        return self.request('put', url=url, params=params)

    def head(self, url, params=None):
        url = self._patch_querystring(url, params)
        return self.request('head', url=url, params=params)

    def delete(self, url, params=None):
        url = self._patch_querystring(url, params)
        return self.request('delete', url=url, params=params)


class Response(object):
    def __init__(self, status_code, url, text, reason, headers):
        self.status_code = status_code
        self.url = url
        self.text = unicode(text)
        self.reason = unicode(reason)
        self.content = str(text)
        self.headers = headers

    @property
    def json(self):
        try:
            return json.loads(self.content)
        except ValueError as e:
            return None
