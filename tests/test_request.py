# -*- coding: utf-8 -*-
from sure import expect
from tests.helpers import server_test_case
from sleepyhollow import (
    SleepyHollow, Response, InvalidUrlError, ConnectionRefusedError
)


@server_test_case
def test_request_api(context):
    sl = SleepyHollow()
    response1 = sl.request('get', context.route_to("/simple"))
    response2 = sl.get(context.route_to("/simple"))

    response1.status_code.should.equal(response2.status_code)
    response1.reason.should.equal(response2.reason)
    response1.text.should.equal(response2.text)
    response1.content.should.equal(response2.content)


@server_test_case
def test_invalid_url(context):
    sl = SleepyHollow()

    sl.get.when.called_with('invalid url').should.throw(
        InvalidUrlError,
        'The url "invalid url" is not valid: You need to inform a scheme')


def test_connection_refused():
    sl = SleepyHollow()
    sl.get.when.called_with('http://blah').should.throw(
        ConnectionRefusedError)


@server_test_case
def test_response(context):
    "Retrieving the response object using the get method"
    sl = SleepyHollow()
    response = sl.get(context.route_to('/simple'))

    # Let's test the types
    response.should.be.a(Response)
    response.url.should.be.a(unicode)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)
    response.json.should.be.none

    # Now let's test the values
    response.url.should.equal(context.route_to('/simple'))
    response.status_code.should.equal(200)
    expect('Very Simple').to.be.within(response.text)


@server_test_case
def test_json_response(context):
    "Retrieving a JSON response object using the get method"
    sl = SleepyHollow()
    response = sl.get(context.route_to('/status-200.json'))

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)

    response.json.should.equal({
        u'success': True,
        u'status': 200,
    })


@server_test_case
def test_response_status_codes(context):
    sl = SleepyHollow()

    response = sl.get(context.route_to('/status-200'))
    response.status_code.should.equal(200)
    response.reason.should.equal('OK')
    expect('Status 200').to.be.within(response.text)

    response = sl.get(context.route_to('/status-404'))
    response.status_code.should.equal(404)
    response.reason.should.equal('Not Found')
    expect('Status 404').to.be.within(response.text)

    response = sl.get(context.route_to('/status-500'))
    response.status_code.should.equal(500)
    response.reason.should.equal('Internal Server Error')
    expect('Status 500').to.be.within(response.text)


@server_test_case
def test_response_headers(context):
    sl = SleepyHollow()

    response = sl.get(context.route_to('/status-200'))

    response.should.have.property('headers').being.a(dict)

    response.headers.should.have.key('Content-Type').being.equal(u'text/html; charset=UTF-8')
    response.headers.should.have.key('Server').being.equal(u'TornadoServer/2.4.1')
    response.headers.should.have.key('Content-Length').being.equal(u'91')
    response.headers.should.have.key('Etag').being.equal(u'"917c97d9437cbd1c1192f2f516e7155183b58232"')
