#include <iostream>
#include <cstring>
#include <QObject>
#include <QWebFrame>
#include <QApplication>

#include <yipit/hollow/hollow.h>
#include <yipit/hollow/error.h>
#include <yipit/hollow/webpage.h>


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
Hollow::getUrlContent(const std::string url) throw (UrlNotLoadedProperly)
{
  QUrl qurl(QString::fromStdString(url));

  page->mainFrame()->setUrl(qurl);
  app->exec();
  if (hasErrors)
    throw UrlNotLoadedProperly(page->getCurrentError().toAscii().data());
  else
    return page->mainFrame()->toHtml().toUtf8().constData();
}
