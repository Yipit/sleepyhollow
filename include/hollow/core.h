// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_CORE_H
#define HOLLOW_CORE_H

#include <string>
#include <map>
#include <QString>
#include <QThread>
#include <QNetworkReply>

class Response;

typedef std::map<std::string, std::string> StringHashMap;
typedef StringHashMap::iterator StringHashMapIterator;

typedef std::map<std::string, Response*> ResponseHashMap;
typedef ResponseHashMap::iterator ResponseHashMapIterator;

// We have to do that, cause the QThread::msleep() method is private :/
class SleeperThread : public QThread {
public:
  static void msleep(unsigned long msecs) {
    QThread::msleep(msecs);
  }
};

#endif  // HOLLOW_CORE_H
