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
    def get(self, *args, **kw):
        _response = super(SleepyHollow, self).get(*args, **kw)

        return Response(
            status_code=_response.status_code,
            content=_response.content,
            reason=_response.reason,
            headers=_response.headers,
            text=_response.text,
        )


class Response(object):
    def __init__(self, status_code, reason, content, text, headers):
        self.status_code = int(status_code)
        self.reason = unicode(reason)
        self.content = str(content)
        self.text = unicode(text)
        self.headers = headers.copy()

    @property
    def json(self):
        try:
            return json.loads(self.content)
        except ValueError as e:
            import ipdb;ipdb.set_trace()

            return None
