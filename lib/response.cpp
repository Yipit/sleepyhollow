#include <cstdlib>
#include <cstring>
#include <hollow/core.h>
#include <hollow/response.h>


Response::Response (int statusCode,
                    const char* url,
                    const char* text,
                    const char* html,
                    const char* reason,
                    StringHashMap& headers)
  : m_statusCode(statusCode)
  , m_text(strdup(text))
  , m_html(strdup(html))
  , m_reason(strdup(reason))
  , m_url(strdup(url))
  , m_headers(headers)
{
}


Response::~Response()
{
  free (m_text);
  free (m_html);
  free (m_reason);
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


const char *
Response::getReason (void)
{
  return m_reason;
}


StringHashMap
Response::getHeaders (void)
{
  return m_headers;
}
