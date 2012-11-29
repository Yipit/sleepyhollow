#include <QObject>
#include <QWebFrame>
#include <QApplication>
#include <QEventLoop>
#include "manager.h"
#include "webpage.h"

QApplication *app = 0;

Manager::Manager(QObject *parent)
  : QObject(parent)
{
  // Mocking the values to pass to QApplication
  int argc = 1;
  char *argv[] = { (char *) "sleepy hollow", 0 };

  // Creating the app that will run untill we get the data
  if (qApp == 0) {
    app = new QApplication(argc, argv);
  }

  loop = new QEventLoop();

  // This must be instantiated *after* the app
  page = new WebPage();

  // This app will die when we finish downloading our stuff
  QObject::connect ((QObject *) page->mainFrame(), SIGNAL(loadFinished(bool)),
                    this, SLOT(proxy()));
}


void
Manager::proxy(void)
{
  loop->exit();
}

Manager::~Manager()
{
  delete page;
}


QString
Manager::getUrlContent(QUrl url)
{
  page->mainFrame()->setUrl(url);
  loop->exec();
  return page->mainFrame()->toHtml();
}
