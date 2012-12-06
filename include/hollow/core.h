// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_CORE_H
#define HOLLOW_CORE_H

#include <string>
#include <map>
#include <QString>
#include <QNetworkReply>

class Response;

typedef std::map<std::string, std::string> StringHashMap;
typedef StringHashMap::iterator StringHashMapIterator;

typedef std::map<std::string, Response*> ResponseHashMap;
typedef ResponseHashMap::iterator ResponseHashMapIterator;

#endif  // HOLLOW_CORE_H
