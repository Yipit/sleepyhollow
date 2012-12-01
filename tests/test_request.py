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
