#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sure import expect
from sleepyhollow import SleepyHollow

http = SleepyHollow()


for x in range(101):
    r = http.get('http://localhost:5000/heavy', {'index': x})
    expect(r.url).to.equal('http://localhost:5000/heavy?index=%d' % x)
    print r, r.url, id(r)
