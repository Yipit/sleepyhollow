// -*- Mode: c++; tab-width: 2 -*-
#include <hollow/jserror.h>

JSError::JSError (std::string message,
                  int line_number,
                  std::string source_id)
  : m_message(message),
    m_line_number(line_number),
    m_source_id(source_id)
{
}

JSError::~JSError()
{
}

const std::string
JSError::getMessage(void) {
  return m_message;
}

const std::string
JSError::getSourceID(void) {
  return m_source_id;
}

int
JSError::getLineNumber(void) {
  return m_line_number;
}
