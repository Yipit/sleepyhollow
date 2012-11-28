#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sure import expect
from sleepy_hollow import SleepyHollow


def test_load():
    ("Loading a page with sleepy hollow")
    sl = SleepyHollow()

    response = sl.load("http://yipit.com")
    expect(response).to.equal("Faking access to: http://yipit.com")
