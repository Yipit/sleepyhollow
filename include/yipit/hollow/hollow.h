// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_H
#define HOLLOW_H

#include <QObject>
#include <QApplication>

#include <yipit/hollow/error.h>
#include <yipit/hollow/webpage.h>

class Hollow : public QObject
{
  Q_OBJECT

public:
  Hollow (QObject *parent=0);
  ~Hollow();
  const char *getUrlContent(const std::string url) throw (UrlNotLoadedProperly);

private slots:
  void proxyExit(bool ok);

private:
  QApplication *app;
  WebPage *page;
  bool hasErrors;
};


#endif
