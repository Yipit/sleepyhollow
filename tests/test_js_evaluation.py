#!/usr/bin/env python
# -*- coding: utf-8 -*-

from tests.helpers import server_test_case
from sleepyhollow import (
    SleepyHollow
)


@server_test_case
def test_decode_string(context):
    "SleepyHollow#evaluate_javascript maps the type `int`"
    sl = SleepyHollow()
    sl.evaluate_javascript("'1'").should.equal('1')
