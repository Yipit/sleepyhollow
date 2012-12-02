#include <iostream>
#include <qdebug.h>
#include <QUrl>
#include <QWebPage>
#include <QWebFrame>
#include <QNetworkReply>
#include <yipit/hollow/webpage.h>
#include <yipit/hollow/error.h>
#include <yipit/hollow/response.h>


WebPage::WebPage(QObject *parent)
  : QWebPage(parent)
  , m_lastResponse(NULL)
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
  QNetworkReply::NetworkError errCode = reply->error();

  // Cleaning up the last response. Maybe it's a good place to track
  // which requests the caller made.
  if (m_lastResponse) {
    delete m_lastResponse;
    m_lastResponse = NULL;
  }

  switch (errCode) {
  case QNetworkReply::NoError:
    // Creating the new response object with the info gathered from this
    // reply
    m_lastResponse = new Response(0, "");
    break;

  case QNetworkReply::ConnectionRefusedError:
    Error::set(Error::CONNECTION_REFUSED, reply->errorString().toAscii().constData());
    break;

  default:
    Error::set(Error::UNKNOWN, reply->errorString().toAscii().constData());
    break;
  }
}


Response *
WebPage::lastResponse()
{
  return m_lastResponse;
}
