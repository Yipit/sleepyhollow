// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_RESPONSE_H
#define HOLLOW_RESPONSE_H

#include <string>

class WebPage;

class Response
{
public:
  Response(int statusCode, const char* text, const char* reason);
  ~Response();
  int getStatusCode(void);
  const char* getText(void);
  const char* getReason(void);

private:
  int m_statusCode;
  char* m_text;
  char* m_reason;
  void setText(const char* text);
  friend class WebPage;
};

#endif  // HOLLOW_RESPONSE_H
