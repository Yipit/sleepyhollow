#include <cstdlib>
#include <cstring>
#include <yipit/hollow/response.h>


Response::Response (int statusCode,
                    const char* url,
                    const char* text,
                    const char* reason,
                    ResponseHeaders& headers)
  : m_statusCode(statusCode)
  , m_text(strdup(text))
  , m_reason(strdup(reason))
  , m_url(strdup(url))
  , m_headers(headers)
{
}


Response::~Response()
{
  free (m_text);
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


void
Response::setText(const char* text)
{
  if (m_text)
    free(m_text);
  m_text = strdup(text);
}


const char *
Response::getReason (void)
{
  return m_reason;
}


ResponseHeaders
Response::getHeaders (void)
{
  return m_headers;
}
