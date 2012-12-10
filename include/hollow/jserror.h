// -*- Mode: c++; tab-width: 2 -*-

#ifndef JS_ERROR_H
#define JS_ERROR_H

#include <string>
#include <map>
#include <QString>
#include <QNetworkReply>
#include <hollow/core.h>


class JSError
{
public:
  JSError(int line_number,
          std::string message,
          std::string source_id);
  ~JSError();
  const std::string getMessage(void);
  const std::string getSourceID(void);
  const char* getMessageCString(void);
  const char* getSourceIDCString(void);

  int getLineNumber(void);

private:
  int m_line_number;
  std::string m_message;
  std::string m_source_id;
};

typedef std::list<JSError> JSErrorList;
typedef JSErrorList::iterator JSErrorListIterator;

#endif  // JS_ERROR_H
