
#include <cstdlib>
#include <cstring>
#include <hollow/core.h>
#include <hollow/response.h>


Response::Response (int statusCode,
                    const char* url,
                    const char* text,
                    const char* html,
                    const char* reason,
                    const char* screenshotData,
                    StringHashMap& headers,
                    utimestamp unix_timestamp)
  : m_statusCode(statusCode)
  , m_text(strdup(text))
  , m_html(strdup(html))
  , m_reason(strdup(reason))
  , m_url(strdup(url))
  , m_screenshotData(strdup(screenshotData))
  , m_headers(headers)
  , m_timestamp(unix_timestamp)
{ }


Response::~Response()
{
  free (m_text);
  free (m_html);
  free (m_reason);
  free (m_screenshotData);
  free (m_url);
}


int
Response::getStatusCode(void)
{
  return m_statusCode;
}


const char*
Response::getURL(void)
{
  return (const char*) m_url;
}


const char *
Response::getText (void)
{
  return m_text;
}


const char *
Response::getHtml (void)
{
  return m_html;
}


void
Response::setText(const char* text)
{
  if (m_text)
    free(m_text);
  m_text = strdup(text);
}


void
Response::setHtml(const char* html)
{
  if (m_html)
    free(m_html);
  m_html = strdup(html);
}

void
Response::setURL(const char* url)
{
  if (m_url)
    free(m_url);
  m_url = strdup(url);
}


const char *
Response::getReason (void)
{
  return m_reason;
}

const char *
Response::getScreenshotData(void)
{
  return m_screenshotData;
}

void
Response::setScreenshotData(const char* screenshotData)
{
  if (m_screenshotData)
    free(m_screenshotData);

  m_screenshotData = strdup(screenshotData);
}


StringHashMap
Response::getHeaders (void)
{
  return m_headers;
}


void
Response::setJSErrors (JSErrorList errors)
{
  m_js_errors = errors;
}


JSErrorList
Response::getJSErrors (void)
{
  return m_js_errors;
}


utimestamp
Response::getTimeStamp (void)
{
  return m_timestamp;
}


void
Response::setRetrievedResources(StringHashMapList retrievedResources)
{
  m_retrievedResources = retrievedResources;
}


StringHashMapList
Response::getRetrievedResources(void)
{
  return m_retrievedResources;
}
