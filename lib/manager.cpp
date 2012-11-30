#include <QObject>
#include <QWebFrame>
#include <QApplication>
#include "manager.h"
#include "webpage.h"

Manager::Manager(QObject *parent)
  : QObject(parent)
{
  // Mocking the values to pass to QApplication
  int argc = 1;
  char *argv[] = { (char *) "sleepy hollow", 0 };

  // Creating the app that will run untill we get the data
  app = new QApplication(argc, argv);

  // This must be instantiated *after* the app
  page = new WebPage();

  // This app will die when we finish downloading our stuff
  // QObject::connect((QObject *) page->mainFrame(), SIGNAL(loadStarted()), this, SLOT(proxyProcessEvents()));
  // QObject::connect((QObject *) page->mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(proxyProcessEvents()));
}


void
Manager::proxyExit(void)
{
    QApplication::exit();
}

void
Manager::proxyProcessEvents(void)
{
    QApplication::processEvents(QEventLoop::AllEvents, 42);
}

Manager::~Manager()
{
    page->deleteLater();
}


QString
Manager::getUrlContent(QUrl url)
{
  page->mainFrame()->setUrl(url);
  QApplication::processEvents(QEventLoop::AllEvents, 42);
  return page->mainFrame()->toHtml();
}
