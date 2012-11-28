#include <iostream>
#include <cstring>
#include <QApplication>

#include "sleepy-hollow.h"
#include "webview.h"

using namespace std;

SleepyHollow::SleepyHollow()
{
}

SleepyHollow::~SleepyHollow()
{
}

char* SleepyHollow::load(char *url) const
{
  int argc = 0;
  char **argv = NULL;
  QApplication app(argc, argv);
  WebView *webview = new WebView();
  webview->setUrl(QUrl(url));
  app.exec();

  return webview->getLoadedContent().toAscii().data();
}
