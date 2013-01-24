#ifndef HOLLOW_PYTHON_MAIN_H
#define HOLLOW_PYTHON_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
  typedef struct {
    PyObject_HEAD;
    Hollow *hollow;
  } SleepyHollow;

#ifdef __cplusplus
}
#endif

#endif // HOLLOW_PYTHON_MAIN_H
