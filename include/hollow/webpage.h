// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_WEBVIEW_H
#define HOLLOW_WEBVIEW_H

#include <QNetworkReply>
#include <QWebPage>
#include <QWebSettings>
#include <hollow/core.h>
#include <hollow/response.h>

class WebPage : public QWebPage
{
  Q_OBJECT

public:
  WebPage(QObject *parent=0);
  Response *lastResponse();
  bool finished();

private slots:
  void handleNetworkReplies(QNetworkReply *);
  bool shouldInterruptJavaScript();
  void attachListener();

public slots:
  void setAsLoaded();

private:
  QNetworkAccessManager *m_networkAccessManager;
  bool m_finished;
  Response *m_lastResponse;
  Response *buildResponseFromNetworkReply(QNetworkReply *reply);
  void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);

};

#endif  // HOLLOW_WEBVIEW_H
