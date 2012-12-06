#include <iostream>
#include <hollow/hollow.h>
#include <hollow/webpage.h>
#include <hollow/webpage.h>

#include <QApplication>
#include <QMainWindow>
#include <QWebView>

int
main (int argc, char **argv)
{
  Hollow hollow;

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << ": <url-to-open>" << std::endl;
    return 1;
  }

  QApplication app(argc, argv);
  app.setApplicationName(QString("SleepyHollow"));
  app.setApplicationVersion(QString("0.0.1"));

  QMainWindow win;
  QWebView view(&win);
  view.load(QUrl(argv[0]));
  win.show();

  return app.exec();
}
