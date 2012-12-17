// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_WEBVIEW_H
#define HOLLOW_WEBVIEW_H

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebPage>
#include <QWebSettings>

#include <hollow/core.h>
#include <hollow/networkaccessmanager.h>
#include <hollow/jserror.h>
#include <hollow/response.h>

class WebPage : public QWebPage
{
  Q_OBJECT

public:
  WebPage(QObject *parent=0, bool disableCache=false);
  Response *lastResponse();
  bool finished();
  bool hasErrors();

public slots:
  void setJSReady();

private slots:
  void handleLoadFinished(bool);
  void handleResourceRequested(const QNetworkRequest&);
  void handleNetworkReplies(QNetworkReply *);
  bool shouldInterruptJavaScript();
  void prepareJS();

private:
  bool m_hasErrors;
  bool m_shouldWaitForJS;
  bool m_jsReady;
  bool m_loadFinished;
  Response *m_lastResponse;
  JSErrorList m_js_errors;
  StringList m_requestedResources;
  NetworkAccessManager *m_networkAccessManager;

  Response *buildResponseFromNetworkReply(QNetworkReply *reply, utimestamp when);
  void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);
};

#endif  // HOLLOW_WEBVIEW_H
