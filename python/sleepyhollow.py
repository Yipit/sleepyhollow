#!/usr/bin/env python
# -*- coding: utf-8 -*-
import json
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
    def request(self, method, *args, **kw):
        response = super(SleepyHollow, self).request(method, *args, **kw)
        return Response(
            status_code=response['status_code'],
            url=response['url'],
            text=response['text'],
            reason=response['reason'],
            headers=response['headers'],
        )

    def get(self, *args, **kw):
        return self.request('get', *args, **kw)


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
