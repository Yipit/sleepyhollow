# -*- coding: utf-8 -*-
from sure import expect
from tests.helpers import server_test_case
from sleepyhollow import SleepyHollow


@server_test_case
def test_querying_elements(context):
    "Using CSS selectors to interact with elements"
    sl = SleepyHollow()
    sl.get(context.route_to("/links"))

    items = sl.dom.query('a').raw()
    expect(items).to.have.length_of(4)

    ny = sl.dom.query('a[href*=new]').one().attr('href')
    expect(ny).to.equal('http://yipit.com/new-york')


@server_test_case
def test_clicking_elements(context):
    "Using CSS selectors to click on elements"
    sl = SleepyHollow()
    sl.get(context.route_to("/links"), config=dict(screenshot=True))

    link = sl.dom.query('a[href*=chicago]').one()
    link.click()

    expect(link.attr("style")).to.equal('color: #668800')
