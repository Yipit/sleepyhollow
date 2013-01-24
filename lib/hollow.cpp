#include <iostream>
#include <QObject>
#include <QWebFrame>
#include <QApplication>

#include <hollow/hollow.h>
#include <hollow/error.h>
#include <hollow/webpage.h>
#include <hollow/response.h>

#include "config.h"

// Mocking the values to pass to QApplication
static int argc = 1;
static char *argv[] = { (char *) "sleepy-hollow", 0 };
static QApplication *app;

Hollow::Hollow(QObject *parent)
  : QObject(parent)
{
  m_lastPage = new WebPage(this);
}

Hollow::~Hollow()
{ }

void
Hollow::setup(void)
{
  app = new QApplication(argc, argv);

  // Setting some cool parameters in our app!
  app->setApplicationName(QString(PACKAGE_NAME));
  app->setApplicationVersion(QString(PACKAGE_VERSION));
}

void
Hollow::teardown(void)
{
  delete app;
  app = 0;
}

void
Hollow::waitForPage()
{

  m_lastPage->setViewportSize(m_lastPage->mainFrame()->contentsSize());
  while (!m_lastPage->finished()) {
    app->processEvents();
    SleeperThread::msleep(0.01);
  }
}

QUrl
Hollow::getValidURL(const char* url)
{
  QUrl qurl(QString::fromStdString(url));
  if (!qurl.isValid() || qurl.scheme().isEmpty()) {
    QString qerr = qurl.errorString();
    QString err("The url \"%1\" is not valid: %2");
    err = err.arg(url, (!qerr.isEmpty() ? qerr : "You need to inform a scheme"));

    // Reporting the error
    Error::set(Error::INVALID_URL, err.toUtf8().data());
  }
  return qurl;
}

QNetworkRequest
Hollow::prepareNetworkRequest(QUrl url, StringHashMap headers)
{
  QNetworkRequest request;

  // setting the request headers coming from the python layer
  StringHashMapIterator headerIter;
  for (headerIter = headers.begin(); headerIter != headers.end(); headerIter++)
    request.setRawHeader(headerIter->first.c_str(), headerIter->second.c_str());

  request.setUrl(url);
  return request;
}

QNetworkAccessManager::Operation
Hollow::parseOperation(QString operation)
{
  QNetworkAccessManager::Operation networkOp = QNetworkAccessManager::UnknownOperation;

  operation = operation.toLower();
  if (operation == "get") {
    networkOp = QNetworkAccessManager::GetOperation;
  } else if (operation == "head") {
    networkOp = QNetworkAccessManager::HeadOperation;
  } else if (operation == "put") {
    networkOp = QNetworkAccessManager::PutOperation;
  } else if (operation == "post") {
    networkOp = QNetworkAccessManager::PostOperation;
  } else if (operation == "delete") {
    networkOp = QNetworkAccessManager::DeleteOperation;
  } else {
    Error::set(Error::INVALID_METHOD, operation.toUtf8().data());
    return networkOp;
  }
  return networkOp;
}

Response *
Hollow::request (const char* method,
                 const char* url,
                 const char* payload,
                 StringHashMap& headers,
                 UsernamePasswordPair& credentials,
                 Config& config)
{
  // First of all, let's see if this url is valid and contains a valid
  // scheme
  QUrl qURL = getValidURL(url);

  // Then set its credentials if any
  qURL.setUserName(QString::fromStdString(credentials.first));
  qURL.setPassword(QString::fromStdString(credentials.second));

  QNetworkRequest request = prepareNetworkRequest(qURL, headers);

  // setting up the page and connecting it's loadFinished signal to our
  // exit function
  if (m_lastPage != NULL)
    delete m_lastPage;

  m_lastPage = new WebPage(this, config);
  m_lastPage->triggerAction(QWebPage::Stop);


  // Setting the payload
  QByteArray body(payload);

  QString action(method);
  QNetworkAccessManager::Operation networkOp = parseOperation(action);
  m_lastPage->mainFrame()->load(request, networkOp, body);

  waitForPage();

  return m_lastPage->lastResponse();
}

QVariant
Hollow::evaluateJavaScript(const char* script)
{
  QString qscript(script);
  return m_lastPage->evaluateJavaScript(qscript);
}

QWebElement
Hollow::document(void)
{
  return m_lastPage->mainFrame()->documentElement();
}

QWebElementCollection
Hollow::querySelectorAll(const char* selector)
{
  return m_lastPage->mainFrame()->findAllElements(QString(selector));
}

QWebElement
Hollow::querySelector(const char* selector)
{
  return m_lastPage->mainFrame()->findFirstElement(QString(selector));
}

Response*
Hollow::lastResponse(void)
{
  return m_lastPage->lastResponse();
}


QImage
Hollow::renderImage(QWebFrame* frame)
{
    QSize contentsSize = frame->contentsSize();
    if (contentsSize.width() < 320) {
      contentsSize.setWidth(320);
    }
    QRect frameRect = QRect(QPoint(0, 0), contentsSize);

    QImage buffer(frameRect.size(), QImage::Format_ARGB32);

    QPainter painter;

    painter.begin(&buffer);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    frame->render(&painter, QRegion(frameRect));

    painter.end();

    return buffer;
}

QByteArray
Hollow::renderPNGBase64(QWebFrame *frame)
{
  QImage rawPageRendering = renderImage(frame);

  QByteArray bytes;
  QBuffer buffer(&bytes);
  buffer.open(QIODevice::WriteOnly);
  rawPageRendering.save(&buffer, "PNG");
  return bytes.toBase64();
}
