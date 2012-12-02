// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_WEBVIEW_H
#define HOLLOW_WEBVIEW_H

#include <QNetworkReply>
#include <QWebPage>
#include <yipit/hollow/response.h>

class WebPage : public QWebPage
{
  Q_OBJECT

public:
  WebPage(QObject *parent=0);
  Response *lastResponse();

private slots:
  void handleNetworkReplies(QNetworkReply *);

private:
  QNetworkAccessManager *m_networkAccessManager;
  Response *m_lastResponse;
  Response *buildResponseFromNetworkReply(QNetworkReply *reply);
};

#endif  // HOLLOW_WEBVIEW_H
