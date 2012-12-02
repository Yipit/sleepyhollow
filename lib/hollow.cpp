#include <iostream>
#include <QObject>
#include <QWebFrame>
#include <QApplication>

#include <yipit/hollow/hollow.h>
#include <yipit/hollow/error.h>
#include <yipit/hollow/webpage.h>
#include <yipit/hollow/response.h>


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


const char *
Hollow::getUrlContent(const std::string url)
{
  QUrl qurl(QString::fromStdString(url));

  page->mainFrame()->setUrl(qurl);
  app->exec();
  if (hasErrors) {
    return NULL;
  } else {
    return page->mainFrame()->toHtml().toUtf8().constData();
  }
}


Response *
Hollow::request (const char* method, const char* url)
{
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

  Q_UNUSED(method);
  return new Response(0, "");
}
