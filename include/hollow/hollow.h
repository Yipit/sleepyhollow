// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_H
#define HOLLOW_H

#include <QObject>
#include <QApplication>

#include <hollow/error.h>
#include <hollow/webpage.h>
#include <hollow/response.h>

class Hollow : public QObject
{
  Q_OBJECT

public:
  Hollow (QObject *parent=0);
  ~Hollow();
  Response* request(const char* method, const char* url, const char* payload, StringHashMap& headers);

private slots:
  void proxyExit(bool ok);

private:
  QApplication *app;
  WebPage *page;
  bool hasErrors;
};


#endif
