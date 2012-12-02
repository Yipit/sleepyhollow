// -*- Mode: c++; tab-width: 2 -*-

#ifndef HOLLOW_ERROR_H
#define HOLLOW_ERROR_H

class Error
{
public:
  ~Error();

  static void set(int code, const char* message);
  static Error* last(void);

  int code(void);
  const char* what(void);

  enum {
    INVALID_URL
  };

private:
  Error(int code, const char* message);
  int m_code;
  char *m_message;
};

// As you can see, it's not thread safe.
static Error *__hollow_error_last = NULL;

#endif  // HOLLOW_ERROR_H
