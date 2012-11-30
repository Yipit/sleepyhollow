#include <iostream>
#include <qdebug.h>
#include <QUrl>
#include <QWebPage>
#include <QWebFrame>
#include "webpage.h"


WebPage::WebPage(QObject *parent)
  : QWebPage(parent)
{
  // Some more configuration to the page and to the page itself
  setForwardUnsupportedContent(true);
}
