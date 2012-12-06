#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sure import expect
from lxml import html as lhtml
from sleepyhollow import SleepyHollow

sl = SleepyHollow()
r = sl.get('http://bananarepublic.gap.com/browse/product.do?pid=331815&locale=en_US&kwid=1&sem=false&sdReferer=http%3A%2F%2Fwww.bananarepublic.com%2Fproducts%2Fpetite-coats-sale.jsp')
dom = lhtml.fromstring(r.html)

imgs = dom.cssselect('#product_image')
if len(imgs) != 1:
    raise AssertionError('damn!')

expect(imgs[0].attrib).to.have.key('src').being.equal('http://www3.assets-gap.com/webcontent/0005/198/392/cn5198392.jpg')
