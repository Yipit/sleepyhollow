#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sleepyhollow import SleepyHollow

http = SleepyHollow()

list_of_things = []

for x in range(101):
    r = http.get('http://localhost:5000/heavy', {'index': x})
    list_of_things.append(id(r))
    print r, r.url

print list(set(list_of_things))
