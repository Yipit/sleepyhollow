#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebPage>

class WebPage : public QWebPage
{
  Q_OBJECT

public:
  WebPage(QObject *parent=0);
};

#endif
