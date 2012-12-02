#include <cstdlib>
#include <cstring>
#include <yipit/hollow/response.h>

Response::Response (int statusCode, const char* text)
{
  m_statusCode = statusCode;
  m_text = strdup (text);
}


Response::~Response()
{
  free (m_text);
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


const char *
Response::getReason (void)
{
  return "";
}
