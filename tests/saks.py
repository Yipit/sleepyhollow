#!/usr/bin/env python
# -*- coding: utf-8 -*-
from lxml import html as lhtml
from sleepyhollow import SleepyHollow


class Scraper(object):
    base_url = 'http://www.saksfifthavenue.com'

    def __init__(self):
        self.http = SleepyHollow()

    def path(self, to):
        if to.startswith('http'):
            return to

        return '/'.join(self.base_url, to.lstrip('/'))

    def get(self, path):
        return self.http.get(self.path(path))

    def get_root_links(self):
        response = self.get('/search/SearchSale.jsp')
        dom = lhtml.fromstring(response.html)
        return [l.attrib['href'] for l in dom.cssselect('a.leftNavText')]

    def get_product_links(self, parent_url):
        response = self.get(parent_url)
        dom = lhtml.fromstring(response.html)
        return [l.attrib['href'] for l in dom.cssselect('a.mainBlackText')]

    def get_product_details(self, product_url):
        name = dom.cssselect('h1.boldBlackText12')

    def scrape(self):
        root_links = self.get_root_links()
