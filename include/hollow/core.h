// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_CORE_H
#define HOLLOW_CORE_H

#include <string>
#include <time.h>
#include <map>
#include <QThread>
#include <QNetworkReply>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
# define C_STRING(x) ((const char*) (x).constData())
#else
# define C_STRING(x) ((const char*) (x).toAscii().constData())
#endif

#define TO_STRING(x) ((x).toString().toStdString().c_str())

typedef time_t utimestamp;

typedef std::map<std::string, std::string> StringHashMap;
typedef StringHashMap::iterator StringHashMapIterator;

typedef std::list<std::string> StringList;
typedef StringList::iterator StringListIterator;

typedef std::list<StringHashMap> StringHashMapList;
typedef StringHashMapList::iterator StringHashMapListIterator;

typedef std::pair<std::string, int> ConfigPair;
typedef std::map<std::string, int> Config;
typedef Config::iterator ConfigIterator;

typedef std::pair<std::string, std::string> UsernamePasswordPair;

// We have to do that, cause the QThread::msleep() method is private :/
class SleeperThread : public QThread {
public:
  static void msleep(unsigned long msecs) {
    QThread::msleep(msecs);
  }
};

#endif  // HOLLOW_CORE_H
