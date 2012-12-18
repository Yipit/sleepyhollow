# -*- coding: utf-8 -*-
from sure import expect
from tests.helpers import server_test_case, qt_version_check
from sleepyhollow import (
    SleepyHollow, Response, InvalidUrlError, ConnectionRefusedError
)


@server_test_case
def test_request_api(context):
    "the get method should return exactly the same thing of request(get)"
    sl = SleepyHollow(cache_enabled=False)
    response1 = sl.request('get', context.route_to("/simple"))
    response2 = sl.get(context.route_to("/simple"))

    response1.status_code.should.equal(response2.status_code)
    response1.reason.should.equal(response2.reason)
    response1.text.should.equal(response2.text)
    response1.content.should.equal(response2.content)


@server_test_case
def test_invalid_url(context):
    "The request method should report an error if the received url is invalid"
    sl = SleepyHollow(cache_enabled=False)
    sl.get.when.called_with('invalid url').should.throw(
        InvalidUrlError,
        'The url "invalid url" is not valid: You need to inform a scheme')


def test_connection_refused():
    "The request method should fail for unreachable urls"
    sl = SleepyHollow(cache_enabled=False)
    sl.get.when.called_with('http://blah').should.throw(
        ConnectionRefusedError)


@server_test_case
def test_response(context):
    "Retrieving the response object using the get method"
    sl = SleepyHollow(cache_enabled=False)
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
    sl = SleepyHollow(cache_enabled=False)
    response = sl.get(context.route_to('/status-200.json'))

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)

    response.json.should.equal({
        u'success': True,
        u'status': 200,
        u'method': 'GET',
    })


@server_test_case
def test_response_status_codes(context):
    "The request method should report the right http status codes"

    sl = SleepyHollow(cache_enabled=False)

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
    "It should be possible to inspect the headers of a response object"
    sl = SleepyHollow(cache_enabled=False)
    response = sl.get(context.route_to('/status-200'))

    response.should.have.property('headers').being.a(dict)

    response.headers.should.have.key('Content-Type').being.equal(
        u'text/html; charset=UTF-8')
    response.headers.should.have.key('Server').being.equal(
        u'TornadoServer/2.4.1')
    response.headers.should.have.key('Content-Length').being.equal(u'91')
    response.headers.should.have.key('Etag').being.equal(
        u'"917c97d9437cbd1c1192f2f516e7155183b58232"')


@server_test_case
def test_get_parameters(context):
    "requesting with GET parameters"
    sl = SleepyHollow(cache_enabled=False)
    response = sl.get(
        context.route_to('/status-200.json'),
        params={'name': 'Gabriel'}
    )

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)

    response.json.should.equal({
        u'success': True,
        u'method': 'GET',
        u'status': 200,
        u'name': u'Gabriel',
    })


@server_test_case
def test_post_parameters(context):
    "requesting with POST parameters"
    sl = SleepyHollow(cache_enabled=False)
    response = sl.post(
        context.route_to('/status-200.json'),
        params={'name': 'Gabriel'},
    )

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)

    response.json.should.equal({
        u'success': True,
        u'method': 'POST',
        u'status': 200,
        u'name': u'Gabriel',
    })


@server_test_case
def test_put_parameters(context):
    "requesting with PUT parameters"
    sl = SleepyHollow(cache_enabled=False)
    response = sl.put(
        context.route_to('/status-200.json'),
        params={'name': 'Gabriel'}
    )

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)

    response.json.should.equal({
        u'success': True,
        u'method': 'PUT',
        u'status': 200,
        u'name': u'Gabriel',
    })


@server_test_case
def test_head_parameters(context):
    "requesting with HEAD parameters"
    sl = SleepyHollow(cache_enabled=False)
    response = sl.head(
        context.route_to('/status-200.json'),
        params={'name': 'Gabriel'},
    )

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)

    response.headers.should.have.key('X-success').being.equal('true')
    response.headers.should.have.key('X-method').being.equal('"HEAD"')
    response.headers.should.have.key('X-status').being.equal('200')
    response.headers.should.have.key('X-name').being.equal('"Gabriel"')


@server_test_case
def test_delete_parameters(context):
    "requesting with DELETE parameters"
    sl = SleepyHollow(cache_enabled=False)
    response = sl.delete(
        context.route_to('/status-200.json'),
        params={'name': 'Gabriel'},
    )

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)

    response.headers.should.have.key('X-success').being.equal('true')
    response.headers.should.have.key('X-method').being.equal('"DELETE"')
    response.headers.should.have.key('X-status').being.equal('200')
    response.headers.should.have.key('X-name').being.equal('"Gabriel"')


@server_test_case
def test_get_sending_headers(context):
    "requesting with GET adding custom headers"
    sl = SleepyHollow(cache_enabled=False)
    response = sl.get(
        context.route_to('/status-200.json'),
        headers={'X-Name': 'Gabriel'}
    )

    # Let's test the types
    response.should.be.a(Response)
    response.status_code.should.be.an(int)
    response.text.should.be.a(unicode)
    response.content.should.be.a(str)

    response.json.should.equal({
        u'success': True,
        u'method': 'GET',
        u'status': 200,
        u'X-Name': u'Gabriel',
    })


@qt_version_check(0x50000)
@server_test_case
def test_getting_js_errors(context):
    "response objects should contain js errors"

    sl = SleepyHollow(cache_enabled=False)
    response = sl.get(context.route_to('/jserror'))

    # Let's test the types
    response.status_code.should.equal(200)
    response.should.have.property('js_errors').being.a(tuple)
    response.js_errors.should.have.length_of(1)
    response.js_errors.should.equal(({
        'line_number': 3,
        'message': u'TypeError: \'undefined\' is not a function (evaluating \'window.intentional_error("javascript errors")\')',
        'source_id': u'http://127.0.0.1:5000/media/js/jserror.js'
    },))
    expect("IT WORKS").to.be.within(response.html)


@server_test_case
def test_requested_resources(context):
    "response object should contain the url of all subrequests"

    sl = SleepyHollow(cache_enabled=False)
    response = sl.get(context.route_to('/fewresources'))

    response.status_code.should.equal(200)
    response.should.have.property('requested_resources').being.a(tuple)
    response.requested_resources.should.have.length_of(4)
    sorted(response.requested_resources).should.equal(sorted((
        'http://127.0.0.1:5000/fewresources',
        'http://127.0.0.1:5000/media/js/jquery-1.8.3.min.js',
        'http://127.0.0.1:5000/media/js/fewresources.js',
        'http://127.0.0.1:5000/media/img/funny.gif',
    )))
