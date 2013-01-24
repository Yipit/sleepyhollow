#ifndef HOLLOW_PYTHON_WEBELEMENT_H
#define HOLLOW_PYTHON_WEBELEMENT_H

#ifdef __cplusplus
extern "C" {
#endif
  typedef struct {
    PyObject_HEAD;
    Hollow *hollow;
    QWebElement *element;
  } WebElement;

#ifdef __cplusplus
}
#endif

#endif  // HOLLOW_PYTHON_WEBELEMENT_H
