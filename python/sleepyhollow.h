#ifdef __cplusplus
extern "C" {
#endif

/* The request object */

typedef struct {
  PyObject_HEAD;
  Hollow *hollow;
} SleepyHollow;

/* The response python object */

typedef struct
{
  PyObject_HEAD;
  Response *response;
  int status_code;
  PyObject *text;
  PyObject *headers;
  char *content;
  char *reason;
} SleepyHollow_Response;

#ifdef __cplusplus
}
#endif
