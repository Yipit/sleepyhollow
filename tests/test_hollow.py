# -*- coding: utf-8 -*-
from sure import expect
from sleepyhollow import SleepyHollow


def test_patch_querystring():
    (u"SleepyHollow._patch_querystring should generate a correct query string")

    sl = SleepyHollow()

    url, params = sl._patch_querystring('http://foo.com/?wee=waa&foo=bar', {'name': 'Lincoln'})
    expect(url).to.equal('http://foo.com/?foo=bar&name=Lincoln&wee=waa')
    expect(params).to.equal({
        'wee': ['waa'],
        'foo': ['bar'],
        'name': 'Lincoln',
    })
