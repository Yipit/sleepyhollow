#include <iostream>
#include <QObject>
#include <QWebFrame>
#include <QApplication>
#include "manager.h"
#include "webpage.h"
#include "error.h"


// Mocking the values to pass to QApplication
static int argc = 1;
static char *argv[] = { (char *) "sleepy-hollow", 0 };

Manager::Manager(QObject *parent)
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
Manager::proxyExit(bool ok)
{
  QApplication::exit();

  // Just marking the last request didn't work. The getUrlContent method
  // will handle that.
  hasErrors = !ok;
}


Manager::~Manager()
{
  delete page;
}


QString
Manager::getUrlContent(QUrl url)
{
  page->mainFrame()->setUrl(url);
  app->exec();
  if (hasErrors) {
    throw UrlNotLoadedProperly(page->getCurrentError().toAscii().data());
  }
  return page->mainFrame()->toHtml();
}
