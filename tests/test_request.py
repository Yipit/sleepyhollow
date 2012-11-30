#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sure import expect
from sleepyhollow import SleepyHollow
from tests.helpers import server_test_case


@server_test_case
def test_load(context):
    ("Loading a page with sleepy hollow")
    sl = SleepyHollow()

    response = sl.load(context.route_to("/simple"))
    expect('Very Simple').to.be.within(response)

    response = sl.load(context.route_to("/simple"))
    expect('Very Simple').to.be.within(response)
