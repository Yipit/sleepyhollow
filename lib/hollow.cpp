#include <iostream>
#include <QObject>
#include <QWebFrame>
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

  // This must be instantiated *after* the app
  page = new WebPage();

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
Hollow::request (const char* method, const char* url)
{
  // Just doing get for now
  Q_UNUSED(method);

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

  page->mainFrame()->setUrl(qurl);

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
