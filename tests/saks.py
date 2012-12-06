#!/usr/bin/env python
# -*- coding: utf-8 -*-
from lxml import html as lhtml
from sleepyhollow import SleepyHollow


class Scraper(object):
    base_url = 'http://m.saks.com'

    def __init__(self):
        self.http = SleepyHollow()

    def path(self, to):
        if to.startswith('http'):
            return to

        return '/'.join([self.base_url, to.lstrip('/')])

    def get(self, path):
        return self.http.get(self.path(path))

    def get_root_links(self):
        print "Getting root links..."
        response = self.get('/eSearch.jsp?sid=127F38CAD8BC&N_Dim=0&bmSingle=N_Dim&N=1553&Ns=P_0_sort')
        dom = lhtml.fromstring(response.html)
        return [l.attrib['href'] for l in dom.cssselect('#left-nav-content > div > a')]

    def get_subcategory_links(self, link):
        print "Getting category links on %r..." % link
        response = self.get(link)
        dom = lhtml.fromstring(response.html)
        return [l.attrib['href'] for l in dom.cssselect('#left-nav-content > div > a')]

    def get_product_links(self, parent_url):
        print "Getting product links on %r..." % parent_url
        response = self.get(parent_url)
        dom = lhtml.fromstring(response.html)
        return [l.attrib['href'] for l in dom.cssselect('.productRow > a')]

    def get_product_details(self, product_url):
        print "Getting product details on %r..." % product_url

        response = self.get(product_url)
        dom = lhtml.fromstring(response.html)
        img = dom.cssselect("#productMainImg")[0]
        name = dom.cssselect('form h1')[0]
        return dict(
            name=name.text.strip(),
            img=img.attrib['src'],
        )

    def scrape(self):
        root_links = self.get_root_links()
        for root in root_links:
            subcategories = self.get_subcategory_links(root)
            for subcat in subcategories:
                products = self.get_product_links(subcat)
                for prod in products:
                    print self.get_product_details(prod)


s = Scraper()
s.scrape()
