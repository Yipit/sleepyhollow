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
    UNKNOWN,
    INVALID_URL,
    CONNECTION_REFUSED,
    INVALID_METHOD
  };

private:
  Error(int code, const char* message);
  int m_code;
  char *m_message;
};

// As you can see, it's not thread safe.
extern Error *__hollow_error_last;

#endif  // HOLLOW_ERROR_H
