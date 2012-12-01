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
  int status_code;
  PyObject *text;
  PyObject *json;
  const char *content;
} SleepyHollow_Response;

#ifdef __cplusplus
}
#endif
