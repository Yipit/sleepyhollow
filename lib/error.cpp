#include <cstdlib>
#include <cstring>
#include <hollow/error.h>

Error *__hollow_error_last = NULL;

Error::Error(int code, const char* message)
  : m_code(code)
  , m_message(strdup(message))
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


void
Error::set(int code, const char* message)
{
  // Cleaning up previous error, if any
  if (__hollow_error_last)
    delete __hollow_error_last;

  // Updating the global state
  __hollow_error_last = new Error(code, message);
}


Error *
Error::last(void)
{
  Error *current;

  // Saving the error that we'll return and cleaning up the global error
  // state before returning the current one.
  current = __hollow_error_last;
  __hollow_error_last = NULL;
  return current;
}
