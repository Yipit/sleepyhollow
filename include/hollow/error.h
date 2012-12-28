// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_ERROR_H
#define HOLLOW_ERROR_H

class Error
{
public:
  ~Error();

  static void set(int code,
                  const char* message,
                  const char* sourceName,
                  int line);

  static Error* last(void);

  int code(void);
  const char* what(void);
  const char* sourceName(void);
  char* line(void);
  int lineno(void);

  enum {
    UNKNOWN,
    INVALID_URL,
    CONNECTION_REFUSED,
    INVALID_METHOD,
    BAD_CREDENTIALS,
    HTTP_ERROR,
    PAGE_ERROR,
    URL_MISMATCH
  };

private:
  Error(int code, const char* message, const char* sourceName, int lineno);
  Error(Error* other);
  int m_code;
  char *m_message;
  char *m_sourceName;
  int m_lineno;
};

#endif  // HOLLOW_ERROR_H
