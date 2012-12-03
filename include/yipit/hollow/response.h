// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_RESPONSE_H
#define HOLLOW_RESPONSE_H

#include <string>
#include <QString>
#include <QHash>
#include <QHashIterator>
#include <QNetworkReply>

class WebPage;

class Response
{
public:
  Response(int statusCodeo, const char* text, const char* reason, QNetworkReply* reply);
  ~Response();
  int getStatusCode(void);
  const char* getText(void);
  const char* getReason(void);
  QHash<QString, QString> getHeaders(void);

private:
  int m_statusCode;
  char* m_text;
  char* m_reason;
  QHash<QString, QString> m_headers;
  void setText(const char* text);
  friend class WebPage;
};

#endif  // HOLLOW_RESPONSE_H
