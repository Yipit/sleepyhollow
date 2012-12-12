// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_COOKIEJAR_H
#define HOLLOW_COOKIEJAR_H

#include <QSettings>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <hollow/core.h>

class CookieJar : public QNetworkCookieJar
{
  Q_OBJECT

public:
  CookieJar(QObject *parent=0);
  bool setCookiesFromUrl(const QList<QNetworkCookie>& cookieList, const QUrl& url);
  QList<QNetworkCookie> cookiesForUrl (const QUrl & url) const;

private:
  QSettings m_cookiePot;
};


#endif  // HOLLOW_COOKIEJAR_H
