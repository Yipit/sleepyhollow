// -*- Mode: c++; tab-width: 2 -*-
#include <hollow/jserror.h>

JSError::JSError(int line_number,
                 std::string message,
                 std::string source_id)
  : m_line_number(line_number)
  , m_message(message)
  , m_source_id(source_id)
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
