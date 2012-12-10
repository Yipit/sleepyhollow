// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_CORE_H
#define HOLLOW_CORE_H

#include <string>
#include <map>
#include <QThread>
#include <QNetworkReply>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
# define C_STRING(x) ((const char*) (x).constData())
#else
# define C_STRING(x) ((const char*) (x).toAscii().constData())
#endif

#define TO_STRING(x) ((x).toString().toStdString().c_str())


typedef std::map<std::string, std::string> StringHashMap;
typedef StringHashMap::iterator StringHashMapIterator;

// We have to do that, cause the QThread::msleep() method is private :/
class SleeperThread : public QThread {
public:
  static void msleep(unsigned long msecs) {
    QThread::msleep(msecs);
  }
};

#endif  // HOLLOW_CORE_H
