// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_RESPONSE_H
#define HOLLOW_RESPONSE_H

#include <string>
#include <map>
#include <QString>
#include <QNetworkReply>
#include <hollow/core.h>
#include <hollow/jserror.h>

class WebPage;

class Response
{
public:
  Response(int statusCode,
           const char* url,
           const char* text,
           const char* html,
           const char* reason,
           StringHashMap& headers,
           utimestamp unix_timestamp);

  ~Response();
  int getStatusCode(void);
  const char* getURL(void);
  const char* getText(void);
  const char* getHtml(void);
  const char* getReason(void);
  StringHashMap getHeaders(void);
  void setJSErrors (JSErrorList errors);
  const JSErrorList getJSErrors (void);
  utimestamp getTimeStamp(void);

private:
  int m_statusCode;
  char* m_text;
  char* m_html;
  char* m_reason;
  char* m_url;
  StringHashMap m_headers;
  JSErrorList m_js_errors;
  utimestamp m_timestamp;
  void setText(const char* text);
  void setHtml(const char* html);
  friend class WebPage;
};

#endif  // HOLLOW_RESPONSE_H
