// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_RESPONSE_H
#define HOLLOW_RESPONSE_H

#include <string>

class Response
{
public:
  Response(int statusCode, const char* text);
  ~Response();
  int getStatusCode(void);
  const char* getText(void);
  const char* getReason(void);

private:
  int m_statusCode;
  char* m_text;
};

#endif  // HOLLOW_RESPONSE_H
