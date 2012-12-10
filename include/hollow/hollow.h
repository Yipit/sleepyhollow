// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_H
#define HOLLOW_H

#include <QObject>
#include <QApplication>

#include <hollow/error.h>
#include <hollow/response.h>

class Hollow : public QObject
{
  Q_OBJECT

public:
  Hollow (QObject *parent=0, bool disableCache=false);
  ~Hollow();
  Response* request(const char* method, const char* url, const char* payload, StringHashMap& headers);

  // The user *Must* call this function before using libhollow
  static void setup(void);
  static void teardown(void);

private:
  bool m_disableCache;
};


#endif
