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
    expect(response).to.look_like('<div class="container"><header class="jumbotron subhead"><div class="row"><div class="span6"><h1>Simple</h1><p class="lead">Very Simple.</p></div></div></header></div>')
