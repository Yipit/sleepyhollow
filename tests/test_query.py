#!/usr/bin/env python
# -*- coding: utf-8 -*-
from mock import Mock
from sure import expect
from sleepyhollow import Query


class ElementStub(object):
    def __init__(self, attrib={}, text="", html=""):
        self.attrib = attrib
        self._text = text
        self._html = html

    def get_text(self):
        return self._text

    def get_outer_html(self):
        return self._html

    def get_attribute(self, name, fallback=None):
        return self.attrib.get(name, fallback)

    def get_attribute_names(self):
        return self.attrib.keys()


def test_query_methods_never_fail_by_returning_itself():
    "Query methods never fail because it always returns itself"

    dom = Mock()
    dom.cssselect.return_value = []
    query = Query(dom)

    expect(query.query('a')).to.be.a(Query)
    expect(query.query('a').attr('href')).to.be.a(Query)


# Testing .attr

def test_query_and_attr_with_no_objects():
    "Query by atributes with no objects"

    dom = Mock()

    dom.cssselect.return_value = []

    query = Query(dom)

    q = query.query('li a').attr('href')
    expect(q).to.be.a(Query)
    expect(q.raw()).to.equal([])


def test_query_and_attr_with_many_objects():
    "Query by atributes with many"

    dom = Mock()

    l1 = ElementStub({'href': 'http://yipit.com'})
    l2 = ElementStub({'href': 'http://github.com'})

    dom.cssselect.return_value = [l1, l2]

    query = Query(dom)

    expect(query.query('li a').attr('href').raw()).to.equal([
        'http://yipit.com',
        'http://github.com',
    ])


def test_query_and_attr_with_one_object():
    "Query by atributes with one object"

    dom = Mock()
    link = ElementStub({'href': 'http://yipit.com'})

    dom.cssselect.return_value = [link]

    query = Query(dom)

    expect(query.query('li a').attr('href').raw()).to.equal(
        'http://yipit.com')


def test_query_attr_one_with_many_objects():
    "Query by atributes with many and calling .one()"

    dom = Mock()
    l1 = ElementStub({'href': 'http://yipit.com'})
    l2 = ElementStub({'href': 'http://github.com'})

    dom.cssselect.return_value = [l1, l2]

    query = Query(dom)

    expect(query.query('li a').attr('href').one()).to.equal(
        'http://yipit.com')


def test_query_attr_one_with_one_object():
    "Query by atributes with one object"

    dom = Mock()
    link = ElementStub({'href': 'http://yipit.com'})

    dom.cssselect.return_value = [link]

    query = Query(dom)

    expect(query.query('li a').attr('href').one()).to.equal(
        'http://yipit.com')


# Testing .text

def test_query_and_text_with_no_objects():
    "Query and retrieve text with no objects"

    dom = Mock()

    dom.cssselect.return_value = []

    query = Query(dom)
    expect(query.query('ul.menu li a').text()).to.equal('')


def test_query_and_text_with_many_objects():
    "Query and retrieve text with many objects"

    dom = Mock()
    l1 = ElementStub(text='  foo  ')
    l2 = ElementStub(text=' \n bar \n  ')

    dom.cssselect.return_value = [l1, l2]

    query = Query(dom)
    expect(query.query('ul.menu li a').text()).to.equal(['foo', 'bar'])


def test_query_and_text_with_one_object():
    "Query and retrieve text with one object"

    dom = Mock()
    l1 = ElementStub(text='  foo  ')

    dom.cssselect.return_value = [l1]

    query = Query(dom)
    expect(query.query('ul.menu li a').text()).to.equal('foo')


# Testing .html

def test_query_and_html_with_no_objects():
    "Query and retrieve text with no objects"

    dom = Mock()

    dom.cssselect.return_value = []

    query = Query(dom)
    expect(query.query('ul.menu li a').html()).to.equal('')


def test_query_and_html_with_many_objects():
    "Query and retrieve text with many objects"

    dom = Mock()
    l1 = ElementStub(html='  foo  ')
    l2 = ElementStub(html=' \n bar \n  ')

    dom.cssselect.return_value = [l1, l2]

    query = Query(dom)
    expect(query.query('ul.menu li a').html()).to.equal(['foo', 'bar'])


def test_query_and_html_with_one_object():
    "Query and retrieve text with one object"

    dom = Mock()
    l1 = ElementStub(html='  foo  ')

    dom.cssselect.return_value = [l1]

    query = Query(dom)
    expect(query.query('ul.menu li a').html()).to.equal('foo')
