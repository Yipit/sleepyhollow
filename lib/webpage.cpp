#include <iostream>
#include <qdebug.h>
#include <QApplication>
#include <QUrl>
#include <QWebPage>
#include <QWebFrame>
#include <QNetworkReply>
#include <hollow/webpage.h>
#include <hollow/error.h>
#include <hollow/response.h>


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

  m_settings = settings();
  m_settings->setDefaultTextEncoding("utf-8");
  m_settings->setFontSize(QWebSettings::MinimumFontSize, 10);
  m_settings->setFontSize(QWebSettings::MinimumLogicalFontSize, 10);
  m_settings->setFontSize(QWebSettings::DefaultFontSize, 12);
  m_settings->setFontSize(QWebSettings::DefaultFixedFontSize, 14);
  m_settings->setAttribute(QWebSettings::JavaEnabled, false);
  m_settings->setAttribute(QWebSettings::JavascriptEnabled, true);
  m_settings->setAttribute(QWebSettings::PluginsEnabled, false);
  m_settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
  m_settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
  m_settings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, false);
  // QUrl css_path("file:///path/to/file.css");
  // m_settings->setUserStyleSheetUrl(css_path);
}


bool
WebPage::shouldInterruptJavaScript() {
  QApplication::processEvents(QEventLoop::AllEvents, 42);
  return false;
}


Response *
WebPage::buildResponseFromNetworkReply(QNetworkReply *reply)
{
  QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute);

  // Not an HTTP error, let's give up
  if (!statusCode.isValid())
    return NULL;

  // Iterating over the headers
  StringHashMap headers;
  foreach (QByteArray headerName, reply->rawHeaderList()) {
    QString key = QString::fromUtf8(headerName);
    QString value = QString::fromUtf8(reply->rawHeader(headerName));
    headers[key.toAscii().data()] = value.toAscii().data();
  }

  // We can't set the content right now, so we'll fill the text with an
  // empty string and let the ::lastResponse() method fill with the
  // right content
  return new Response(statusCode.toInt(),
                      reply->url().toString().toAscii().constData(),
                      "",
                      "",
                      reason.toString().toAscii().constData(),
                      headers);
}


void
WebPage::handleNetworkReplies(QNetworkReply *reply)
{
  // Making sure we're handling the right url
  if ((mainFrame()->url() != reply->url()))
    return;

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
    m_lastResponse = buildResponseFromNetworkReply(reply);
    break;

  case QNetworkReply::ConnectionRefusedError:
    Error::set(Error::CONNECTION_REFUSED, reply->errorString().toAscii().constData());
    break;

  default:
    // Maybe we can create a response when an error happens. If the
    // reply does not have all the data needed to create it, the method
    // buildResponseFromNetworkReply() will return NULL.
    if ((m_lastResponse = buildResponseFromNetworkReply(reply)) == NULL)
      Error::set(Error::UNKNOWN, reply->errorString().toAscii().constData());
    break;
  }
}


Response *
WebPage::lastResponse()
{
  if (m_lastResponse) {
    // We're setting this value here because we can't get the response
    // text before this point. We have to wait the page to be processed
    // by the html engine after finishing the request.
    m_lastResponse->setHtml(mainFrame()->toHtml().toUtf8().constData());
    m_lastResponse->setText(mainFrame()->toPlainText().toUtf8().constData());
  }
  return m_lastResponse;
}
