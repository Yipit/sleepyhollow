// -*- Mode: c++; tab-width: 2 -*-

#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QNetworkReply>
#include <QWebPage>

class WebPage : public QWebPage
{
  Q_OBJECT

public:
  WebPage(QObject *parent=0);
  QString getCurrentError(void);

private slots:
  void handleNetworkReplies(QNetworkReply *);

private:
  QNetworkAccessManager *m_networkAccessManager;
  QString m_currentError;
};

#endif
