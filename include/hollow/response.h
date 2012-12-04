// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_RESPONSE_H
#define HOLLOW_RESPONSE_H

#include <string>
#include <map>
#include <QString>
#include <QNetworkReply>

typedef std::map<std::string, std::string> StringHashMap;

typedef StringHashMap::iterator StringHashMapIterator;

class WebPage;

class Response
{
public:
  Response(int statusCode,
           const char* url,
           const char* text,
           const char* reason,
           StringHashMap& headers);
  ~Response();
  int getStatusCode(void);
  const char* getURL(void);
  const char* getText(void);
  const char* getReason(void);
  StringHashMap getHeaders(void);

private:
  int m_statusCode;
  char* m_text;
  char* m_reason;
  char* m_url;
  StringHashMap m_headers;
  void setText(const char* text);
  friend class WebPage;
};

#endif  // HOLLOW_RESPONSE_H
