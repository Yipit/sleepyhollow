#include <iostream>
#include <QObject>
#include <QWebFrame>
#include <QNetworkRequest>
#include <QApplication>

#include <hollow/hollow.h>
#include <hollow/error.h>
#include <hollow/webpage.h>
#include <hollow/response.h>

#include "config.h"


// Mocking the values to pass to QApplication
static int argc = 1;
static char *argv[] = { (char *) "sleepy-hollow", 0 };

Hollow::Hollow(QObject *parent)
  : QObject(parent)
  , app(argc, argv)
{
  // Setting some cool parameters in our app!
  app.setApplicationName(QString(PACKAGE_NAME));
  app.setApplicationVersion(QString(PACKAGE_VERSION));
}

Hollow::~Hollow()
{ }


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

  // setting up the page and connecting it's loadFinished signal to our
  // exit function
  WebPage page;

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

  // setting the request headers coming from the python layer
  for (headerIterator = headers.begin(); headerIterator != headers.end(); headerIterator++){
    request.setRawHeader(QString(headerIterator->first.c_str()).toAscii(),
                         QByteArray(headerIterator->second.c_str()));
  }

  QByteArray body(payload);
  request.setUrl(qurl);
  page.mainFrame()->load(request, networkOp, body);
  page.setViewportSize(page.mainFrame()->contentsSize());

  // Mainloop
  while (!page.finished()) {
    app.processEvents();
    SleeperThread::msleep(0.01);
  }

  if (page.hasErrors()) {
    // The error was properly set in the WebPage::handleNetworkReplies()
    // method, so we just need to return NULL to notify the caller that
    // something didn't work.
    return NULL;
  } else {
    // Yay! Let's return the response object created by the webpage
    // after receiving a network reply.
    Response *resp = page.lastResponse();
    return resp;
  }
}
