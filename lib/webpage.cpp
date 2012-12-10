#include <iostream>
#include <QApplication>
#include <QUrl>
#include <QWebPage>
#include <QWebFrame>
#include <QNetworkReply>

#include <hollow/core.h>
#include <hollow/networkaccessmanager.h>
#include <hollow/webpage.h>
#include <hollow/error.h>
#include <hollow/response.h>
#include <hollow/jserror.h>


WebPage::WebPage(QObject *parent)
  : QWebPage(parent)
  , m_hasErrors(false)
  , m_loadFinished(false)
  , m_lastResponse(NULL)
{
  // Some more configuration to the page and to the page itself
  setForwardUnsupportedContent(true);

  // Setting the internal finished flag to true when the page finished
  // loading. We use both this flag and the number of resources being
  // downloaded to ensure that the page is loaded
  connect(this, SIGNAL(loadFinished(bool)),
          this, SLOT(handleLoadFinished(bool)),
          Qt::DirectConnection);

  // Everytime a new resource is requested, we increment our internal
  // counter and we won't return untill all the requested resources are
  // downloaded. This is why we have to extend the network access
  // manager and add our custom instance here
  m_networkAccessManager = new NetworkAccessManager(this);
  setNetworkAccessManager(m_networkAccessManager);
  connect(m_networkAccessManager, SIGNAL(resourceRequested(const QNetworkRequest&)),
          this, SLOT(handleResourceRequested(const QNetworkRequest&)),
          Qt::DirectConnection);

  // We need this object to track the replies and get info when the
  // request does not work
  connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply *)),
          this, SLOT(handleNetworkReplies(QNetworkReply *)),
          Qt::DirectConnection);

  // Setting the default style for our page
  settings()->setDefaultTextEncoding("utf-8");
  settings()->setFontSize(QWebSettings::MinimumFontSize, 10);
  settings()->setFontSize(QWebSettings::MinimumLogicalFontSize, 10);
  settings()->setFontSize(QWebSettings::DefaultFontSize, 12);
  settings()->setFontSize(QWebSettings::DefaultFixedFontSize, 14);
  settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
  settings()->setAttribute(QWebSettings::JavaEnabled, false);
  settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
  settings()->setAttribute(QWebSettings::PluginsEnabled, false);
  settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
  settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
  settings()->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);
}

// -- Public API --

bool
WebPage::finished()
{
  return m_loadFinished && m_requestedResources.isEmpty();
}


bool
WebPage::hasErrors()
{
  return m_hasErrors;
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
    m_lastResponse->setJSErrors(m_js_errors);
  }
  return m_lastResponse;
}


// -- Slots --


bool
WebPage::shouldInterruptJavaScript() {
  QApplication::processEvents(QEventLoop::AllEvents, 42);
  return false;
}


void
WebPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID)
{
  m_js_errors.push_back(JSError(lineNumber, message.toStdString(), sourceID.toStdString()));
}


void
WebPage::handleLoadFinished(bool ok)
{
  m_loadFinished = true;
  m_hasErrors = !ok;
}


void
WebPage::handleResourceRequested(const QNetworkRequest& request)
{
  m_requestedResources.append(request.url());
}


void
WebPage::handleNetworkReplies(QNetworkReply *reply)
{
  time_t now;
  now = time(NULL);

  // Keeping the list of requested resources updated
  m_requestedResources.removeOne(reply->url());

  // Making sure we're handling the right url
  QUrl url = mainFrame()->url();
  if (url.isEmpty())
    url = mainFrame()->requestedUrl();
  if (url != reply->url())
    return;

  // Cleaning up the last response. Maybe it's a good place to track
  // which requests the caller made.
  if (m_lastResponse) {
    delete m_lastResponse;
    m_lastResponse = NULL;
  }

  // Error handling
  QNetworkReply::NetworkError errCode = reply->error();
  switch (errCode) {
  case QNetworkReply::NoError:
    // Creating the new response object with the info gathered from this
    // reply
    m_lastResponse = buildResponseFromNetworkReply(reply, now);
    break;

  case QNetworkReply::ConnectionRefusedError:
    Error::set(Error::CONNECTION_REFUSED, C_STRING(reply->errorString()));
    break;

  default:
    // Maybe we can create a response when an error happens. If the
    // reply does not have all the data needed to create it, the method
    // buildResponseFromNetworkReply() will return NULL.
    if ((m_lastResponse = buildResponseFromNetworkReply(reply, now)) == NULL)
      Error::set(Error::UNKNOWN, C_STRING(reply->errorString()));
    break;
  }
}


// -- Helper methods/private API --


Response *
WebPage::buildResponseFromNetworkReply(QNetworkReply *reply, utimestamp when)
{
  QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute);

  // Not an HTTP error, let's give up
  if (!statusCode.isValid())
    return NULL;

  // Iterating over the headers
  StringHashMap headers;
  foreach (QByteArray headerName, reply->rawHeaderList())
    headers[headerName.constData()] = reply->rawHeader(headerName).constData();

  // We can't set the content right now, so we'll fill the text with an
  // empty string and let the ::lastResponse() method fill with the
  // right content
  return new Response(statusCode.toInt(),
                      TO_STRING(reply->url()),
                      "",
                      "",
                      TO_STRING(reason),
                      headers,
                      when);
}
