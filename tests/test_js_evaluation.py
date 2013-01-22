#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sure import expect
from tests.helpers import server_test_case
from sleepyhollow import (
    SleepyHollow, InvalidJSONError
)


@server_test_case
def test_decode_string(context):
    "SleepyHollow#evaluate_javascript maps the type `string`"
    sl = SleepyHollow()
    sl.evaluate_javascript(r'"GABRIEL\'s".toLowerCase()').should.equal("gabriel's")


@server_test_case
def test_decode_string_with_double_quotes(context):
    "SleepyHollow#evaluate_javascript maps the type `string` with double quotes"
    sl = SleepyHollow()
    sl.evaluate_javascript(r'"\"NICE\""').should.equal('"NICE"')


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


@server_test_case
def test_decode_complex_object(context):
    "SleepyHollow#evaluate_javascript maps the type `dict`"
    sl = SleepyHollow()
    evaluated = sl.evaluate_javascript(r'''(function(){
        var data = {};
        data["name"] = "Gabriel Falcão \"gabrielfalcao\"";
        data["github"] = "http://github.com/gabrielfalcao";
        data["projects"] = ["cello", "sleepyhollow"];
        data["coder"] = true;
        data["age"] = 24;
        data["weight"] = 77.5;
        data["more"] = null;
        return data;
    })()''')

    expect(evaluated).to.equal({
        u'name': u'Gabriel Falcão "gabrielfalcao"',
        u'github': u'http://github.com/gabrielfalcao',
        u'projects': [u'cello', u'sleepyhollow'],
        u'coder': True,
        u'age': 24,
        u'weight': 77.5,
        u'more': "",
    })


@server_test_case
def test_error_handling(context):
    "SleepyHollow#evaluate_javascript handles errors"
    sl = SleepyHollow()

    script1 = r'''(function(){
        return foo;
    })()'''

    expect(sl.evaluate_javascript).when.called_with(script1).to.throw(
        InvalidJSONError,
        "'ReferenceError: Can't find variable: foo' undefined:2"
    )
    sl.evaluate_javascript("'SUCCESS!'").should.equal("SUCCESS!")
