#include <iostream>
#include <QObject>
#include <QWebFrame>
#include <QNetworkRequest>
#include <QApplication>

#include <hollow/hollow.h>
#include <hollow/error.h>
#include <hollow/webpage.h>
#include <hollow/response.h>


// Mocking the values to pass to QApplication
static int argc = 1;
static char *argv[] = { (char *) "sleepy-hollow", 0 };

Hollow::Hollow(QObject *parent)
  : QObject(parent)
  , hasErrors(false)
{
  // Creating the app that will run untill we get the data
  app = new QApplication(argc, argv);
  app->setApplicationName(QString("SleepyHollow"));
  app->setApplicationVersion(QString("0.0.1"));
  // This must be instantiated *after* the app
  page = new WebPage();

  // setting up the page
  // This app will die when we finish downloading our stuff
  QObject::connect((QObject *) page->mainFrame(), SIGNAL(loadFinished(bool)),
                   this, SLOT(proxyExit(bool)));
}


void
Hollow::proxyExit(bool ok)
{
  QApplication::exit();

  // Just marking the last request didn't work. The getUrlContent method
  // will handle that.
  hasErrors = !ok;
}


Hollow::~Hollow()
{
  delete page;
}


Response *
Hollow::request (const char* method, const char* url, const char* payload, StringHashMap& headers)
{
  QString operation(method);
  QNetworkRequest request;
  StringHashMapIterator headerIterator;

  // First of all, let's see if this url is valid and contains a valid
  // scheme
  QUrl qurl(QString::fromStdString(url));
  if (!qurl.isValid() || qurl.scheme().isEmpty()) {
    QString qerr = qurl.errorString();
    QString err("The url \"%1\" is not valid: %2");
    err = err.arg(url, (!qerr.isEmpty() ? qerr : "You need to inform a scheme"));

    // Reporting the error
    Error::set(Error::INVALID_URL, err.toUtf8().data());
    return NULL;
  }

  page->triggerAction(QWebPage::Stop);

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
    return NULL;
  }

  /* setting the request headers coming from the python layer */
  for (headerIterator = headers.begin(); headerIterator != headers.end(); headerIterator++){
    request.setRawHeader(QString(headerIterator->first.c_str()).toAscii(),
                         QByteArray(headerIterator->second.c_str()));
  }

  QByteArray body(payload);
  request.setUrl(qurl);
  page->mainFrame()->load(request, networkOp, body);
  page->setViewportSize(page->mainFrame()->contentsSize());
  // This app will exit when the webpage fires the loadFinished()
  // signal. See proxyExit().
  app->exec();

  if (hasErrors) {
    // The error was properly set in the WebPage::handleNetworkReplies()
    // method, so we just need to return NULL to notify the caller that
    // something didn't work.
    return NULL;
  } else {
    // Yay! Let's return the response object created by the webpage
    // after receiving a network reply.
    return page->lastResponse();
  }
}
