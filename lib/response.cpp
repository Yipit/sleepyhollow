#include <cstdlib>
#include <cstring>
#include <yipit/hollow/response.h>


Response::Response (int statusCode, const char* text, const char* reason, ResponseHeaders& headers)
{
  // The basic stuff
  m_statusCode = statusCode;
  m_text = strdup (text);
  m_reason = strdup (reason);
  m_headers = headers;
}


Response::~Response()
{
  free (m_text);
  free (m_reason);
}


int
Response::getStatusCode(void)
{
  return m_statusCode;
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
