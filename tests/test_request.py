#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sure import expect
from sleepy_hollow import SleepyHollow
from tests.helpers import server_test_case


@server_test_case
def test_load(context):
    ("Loading a page with sleepy hollow")
    sl = SleepyHollow()

    response = sl.load(context.route_to("/simple"))
    expect(response).to.equal("Faking access to: http://yipit.com")
