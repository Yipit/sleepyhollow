#include <QObject>
#include <QWebFrame>
#include <QApplication>
#include "manager.h"
#include "webpage.h"


// Mocking the values to pass to QApplication
static int argc = 1;
static char *argv[] = { (char *) "sleepy-hollow", 0 };

Manager::Manager(QObject *parent)
  : QObject(parent)
{
  // Creating the app that will run untill we get the data
  app = new QApplication(argc, argv);

  // This must be instantiated *after* the app
  page = new WebPage();

  // This app will die when we finish downloading our stuff
  QObject::connect((QObject *) page->mainFrame(), SIGNAL(loadFinished(bool)),
                   this, SLOT(proxyExit()));
}


void
Manager::proxyExit(void)
{
  QApplication::exit();
}


Manager::~Manager()
{
  page->deleteLater();
}


QString
Manager::getUrlContent(QUrl url)
{
  page->mainFrame()->setUrl(url);
  app->exec();
  return page->mainFrame()->toHtml();
}
