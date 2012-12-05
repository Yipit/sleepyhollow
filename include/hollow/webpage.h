// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_WEBVIEW_H
#define HOLLOW_WEBVIEW_H

#include <QNetworkReply>
#include <QWebPage>
#include <QWebSettings>
#include <hollow/response.h>

class WebPage : public QWebPage
{
  Q_OBJECT

public:
  WebPage(QObject *parent=0);
  Response *lastResponse();

private slots:
  void handleNetworkReplies(QNetworkReply *);
  bool shouldInterruptJavaScript();

private:
  QNetworkAccessManager *m_networkAccessManager;
  QWebSettings *m_settings;
  Response *m_lastResponse;
  Response *buildResponseFromNetworkReply(QNetworkReply *reply);
};

#endif  // HOLLOW_WEBVIEW_H
