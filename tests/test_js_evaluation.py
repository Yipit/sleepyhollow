#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sure import expect
from tests.helpers import server_test_case
from sleepyhollow import (
    SleepyHollow, InvalidJSONError
)

from _sleepyhollow import SleepyHollow as _SleepyHollow


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
    sl.evaluate_javascript('(function(){return {name: "GABRIEL FALCÃO".toLowerCase()}})();').should.equal({'name': 'gabriel falcão'.decode('utf-8')})


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
        'name': 'Gabriel Falcão "gabrielfalcao"'.decode('utf-8'),
        'github': u'http://github.com/gabrielfalcao',
        'projects': [u'cello', u'sleepyhollow'],
        'coder': True,
        'age': 24,
        'weight': 77.5,
        'more': "",
    })


@server_test_case
def test_js_evaluation_works_with_custom_codec(context):
    ("SleepyHollow#evaluate_javascript should work with unicode "
     "objects that were already encoded with a preferred codec such as latin1")

    sl = SleepyHollow()
    latin1_unicode = '({"poem": "My ♥ is beating fast"})'.decode("latin1")
    sl.evaluate_javascript(latin1_unicode).should.equal({
        "poem": "My ♥ is beating fast".decode("latin1")
    })


@server_test_case
def test_js_evaluation_raises_exception_upon_sending_nonunicode(context):
    ("_sleepyhollow.SleepyHollow#evaluate_javascript should raise a "
     "TypeError if the given object is not an unicode object")

    _sl = _SleepyHollow()
    _sl.evaluate_javascript.when.called_with({}).should.throw(
        TypeError,
        "SleepyHollow.evaluate_javascript takes an unicode object as parameter, got a dict instead")


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
