#include <iostream>
#include <qdebug.h>
#include <QUrl>
#include <QWebPage>
#include <QWebFrame>
#include <QNetworkReply>
#include <yipit/hollow/webpage.h>


WebPage::WebPage(QObject *parent)
  : QWebPage(parent)
{
  // Some more configuration to the page and to the page itself
  setForwardUnsupportedContent(true);

  // We need this object to track the replies and get info when the
  // request does not work
  m_networkAccessManager = networkAccessManager();
  connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply *)),
          this, SLOT(handleNetworkReplies(QNetworkReply *)));
}


void
WebPage::handleNetworkReplies(QNetworkReply *reply)
{
  m_currentError = reply->errorString();
}


QString
WebPage::getCurrentError(void)
{
  return m_currentError;
}
