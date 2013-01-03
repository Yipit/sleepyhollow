#!/usr/bin/env python
# -*- coding: utf-8 -*-

from tests.helpers import server_test_case
from sleepyhollow import (
    SleepyHollow
)


@server_test_case
def test_decode_string(context):
    "SleepyHollow#evaluate_javascript maps the type `string`"
    sl = SleepyHollow()
    sl.evaluate_javascript("'GABRIEL'.toLowerCase()").should.equal("gabriel")


@server_test_case
def test_decode_float(context):
    "SleepyHollow#evaluate_javascript maps the type `float`"
    sl = SleepyHollow()
    sl.evaluate_javascript("1.4").should.equal(1.4)


@server_test_case
def test_decode_int(context):
    "SleepyHollow#evaluate_javascript maps the type `int`"
    sl = SleepyHollow()
    sl.evaluate_javascript("1").should.equal(1)


@server_test_case
def test_decode_list(context):
    "SleepyHollow#evaluate_javascript maps the type `list`"
    sl = SleepyHollow()
    sl.evaluate_javascript("['gabriel', 'falcao']").should.equal(['gabriel', 'falcao'])


@server_test_case
def test_decode_dict(context):
    "SleepyHollow#evaluate_javascript maps the type `dict`"
    sl = SleepyHollow()
    sl.evaluate_javascript('(function(){return {name: "GABRIEL FALCÃO".toLowerCase()}})();').should.equal({'name': u'gabriel falcão'})
