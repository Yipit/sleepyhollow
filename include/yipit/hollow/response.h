// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_RESPONSE_H
#define HOLLOW_RESPONSE_H

#include <string>
#include <map>
#include <QString>
#include <QNetworkReply>

typedef std::map<std::string, std::string> ResponseHeaders;

typedef ResponseHeaders::iterator ResponseHeadersIterator;

class WebPage;

class Response
{
public:
  Response(int statusCodeo, const char* text, const char* reason, ResponseHeaders& headers);
  ~Response();
  int getStatusCode(void);
  const char* getText(void);
  const char* getReason(void);
  ResponseHeaders getHeaders(void);

private:
  int m_statusCode;
  char* m_text;
  char* m_reason;
  ResponseHeaders m_headers;
  void setText(const char* text);
  friend class WebPage;
};

#endif  // HOLLOW_RESPONSE_H
