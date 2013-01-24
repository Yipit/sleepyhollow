#ifndef HOLLOW_PYTHON_HELPERS_H
#define HOLLOW_PYTHON_HELPERS_H

#include <Python.h>
#include <structmember.h>

#include <hollow/hollow.h>
#include <hollow/jserror.h>
#include <hollow/error.h>
#include <hollow/response.h>
#include "core.h"
#include "webelement.h"
#include "sleepyhollow.h"

StringHashMap pydict_to_string_hash_map(PyObject *dict);
Config pydict_to_config(PyObject *dict);
UsernamePasswordPair pytuple_to_credentials(PyObject *tuple, int size);
PyObject *string_hash_map_to_pydict(StringHashMap map);
PyObject *jserror_to_dict(JSError error);
PyObject *js_error_list_to_pytuple(JSErrorList errors);
PyObject *stringlist_to_python_tuple(StringList list);
PyObject *webelement_from_hollow_and_qwebelement(Hollow *hollow, const QWebElement& element);
PyObject *qvariant_to_pyobject(QVariant variant);
const char *pyunicode_to_utf8_bytestring(PyObject *unicode_string, const char *source);
PyObject *pystring_utf8_from_char_pointer(const char* data);
#endif
