#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sure import expect
from sleepyhollow import SleepyHollow, Response
from tests.helpers import server_test_case


@server_test_case
def test_load(context):
    ("Loading a page with sleepy hollow")
    sl = SleepyHollow()

    response = sl.load(context.route_to("/simple"))
    expect('Very Simple').to.be.within(response)

    response = sl.load(context.route_to("/simple"))
    expect('Very Simple').to.be.within(response)


@server_test_case
def test_request_api(context):
    sl = SleepyHollow()
    response1 = sl.request('get', context.route_to("/simple"))
    response2 = sl.get(context.route_to("/simple"))

    response1.status_code.should.equal(response2.status_code)
    response1.text.should.equal(response2.text)
    response1.content.should.equal(response2.content)
    response1.json.should.equal(response2.json)


@server_test_case
def test_response(context):
    "Retrieving the response object using the get method"
    sl = SleepyHollow()
    response = sl.get(context.route_to('/simple'))

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)
    response.json.should.be.none

    # Now let's test the values
    response.status_code.should.equal(200)
    expect('Very Simple').to.be.within(response.text)


