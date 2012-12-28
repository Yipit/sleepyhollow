#include <cstdlib>
#include <cstring>
#include <hollow/error.h>
#include <QDebug>

static Error *__hollow_error_last = NULL;

Error::Error(int code,
             const char* message,
             const char* sourceName,
             int lineno)
  : m_code(code)
  , m_message(strdup(message))
  , m_sourceName(strdup(sourceName))
  , m_lineno(lineno)
{
}

Error::Error(Error* other)
  : m_code(other->code())
  , m_message(strdup(other->what()))
  , m_sourceName(strdup(other->sourceName()))
  , m_lineno(other->lineno())
{
}


Error::~Error ()
{
  if (m_message)
    free (m_message);
}


int
Error::code(void)
{
  return m_code;
}


const char *
Error::what(void)
{
  return (const char *) m_message;
}

const char *
Error::sourceName(void)
{
  return (const char *) m_sourceName;
}

char *
Error::line(void)
{
  char *buffer = NULL;
  buffer = (char *) malloc(sizeof(char) * 3);

  sprintf(buffer, "%d", lineno());
  return buffer;
}

int
Error::lineno(void)
{
  return m_lineno;
}



void
Error::set(int code,
           const char* message,
           const char* sourceName,
           int lineno)
{
  if (__hollow_error_last == NULL)
    delete __hollow_error_last;
  else {
    qDebug() << "SHIT!\n" << message << "\n" << sourceName << "\n";
    return;
  }
  __hollow_error_last = new Error(code, message, sourceName, lineno);
}


Error *
Error::last(void)
{
  if (__hollow_error_last != NULL) {
    Error *err = new Error(__hollow_error_last);
    delete __hollow_error_last;
    __hollow_error_last = NULL;
    return err;
  }
  return NULL;
}
