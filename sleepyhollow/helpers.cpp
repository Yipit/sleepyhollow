#include <Python.h>
#include <structmember.h>

#include <hollow/hollow.h>
#include <hollow/jserror.h>
#include <hollow/error.h>
#include <hollow/response.h>
#include "core.h"
#include "webelement.h"
#include "sleepyhollow.h"

/* using recursion depth to avoid evaluating deserializing js objects
   that are too big */
#define HOLLOW_MAX_RECURSION_DEPTH 50
static int recursion_depth = 0;


StringHashMap
pydict_to_string_hash_map(PyObject *dict)
{
  StringHashMap ret;
  PyObject *key, *value;
  Py_ssize_t pos = 0;
  while (PyDict_Next(dict, &pos, &key, &value))
    ret[PyString_AsString(key)] = PyString_AsString(value);
  return ret;
}

Config
pydict_to_config(PyObject *dict)
{
  Config ret;
  PyObject *key, *value;
  Py_ssize_t pos = 0;
  while (PyDict_Next(dict, &pos, &key, &value))
    ret[PyString_AsString(key)] = PyBool_Check(value) && value == Py_True;
  return ret;
}

UsernamePasswordPair
pytuple_to_credentials(PyObject *tuple, int size)
{
  UsernamePasswordPair creds;

  if (size == 2) {
    PyObject *username = NULL;
    PyObject *password = NULL;

    username = PyTuple_GetItem(tuple, 0);
    password = PyTuple_GetItem(tuple, 1);

    creds.first = std::string(PyString_AsString(username));
    creds.second = std::string(PyString_AsString(password));
  } else {
    creds.first = "";
    creds.second = "";
  }

  return creds;
}

PyObject *
string_hash_map_to_pydict(StringHashMap map)
{
  StringHashMapIterator iterator;
  PyObject *dict = PyDict_New ();
  for (iterator = map.begin(); iterator != map.end(); iterator++)
    PyDict_SetItemString(dict, iterator->first.c_str(),
                         PyUnicode_FromString(iterator->second.c_str()));
  return dict;
}

PyObject *
jserror_to_dict(JSError error)
{
  /* Takes a JSError instance and turn it into a python dictionary */

  PyObject *dict = PyDict_New();
  PyDict_SetItemString(dict, C_STR("message"),
                       PyUnicode_FromString(error.getMessageCString()));
  PyDict_SetItemString(dict, C_STR("source_id"),
                       PyUnicode_FromString(error.getSourceIDCString()));
  PyDict_SetItemString(dict, C_STR("line_number"),
                       PyInt_FromLong(error.getLineNumber()));
  return dict;
}

PyObject *
js_error_list_to_pytuple(JSErrorList errors)
{
  int pos;
  JSErrorListIterator iter;
  PyObject *tuple = PyTuple_New(errors.size ());
  for (iter = errors.begin(), pos = 0; iter != errors.end(); iter++, pos++)
    PyTuple_SetItem(tuple, pos, jserror_to_dict(*iter));
  return tuple;
}

PyObject *
stringlist_to_python_tuple(StringList list)
{
  int i;
  StringListIterator iter;
  PyObject *tuple = PyTuple_New(list.size ());
  for (iter = list.begin(), i = 0; iter != list.end(); iter++, i++)
    PyTuple_SetItem(tuple, i, PyString_FromString((*iter).c_str()));
  return tuple;
}

PyObject *
webelement_from_hollow_and_qwebelement(Hollow *hollow, const QWebElement& element)
{
  WebElement *ret = NULL;
  ret = PyObject_New(WebElement, &WebElementType);
  if (ret == NULL) {
    Py_RETURN_NONE;
  }

  ret->hollow = hollow;
  ret->element = new QWebElement(element);
  return (PyObject *)ret;
}

PyObject *
qvariant_to_pyobject(QVariant variant)
{
  PyObject *return_value = NULL;
  QStringList qslist;
  QVariantList list;
  QVariantMap map;

  if (recursion_depth >= HOLLOW_MAX_RECURSION_DEPTH) {
    // if the recursion depth is too big it returns None
    Py_RETURN_NONE;
  }
  recursion_depth++;

  switch (variant.type()) {
  case QVariant::Bool:
    return_value = variant.toBool() ? Py_True : Py_False;
    Py_INCREF(return_value);
    break;
  case QVariant::Int:
  case QVariant::UInt:
    return_value = PyInt_FromLong(variant.toInt());
    break;
  case QVariant::Double:
  case QVariant::LongLong:
  case QVariant::ULongLong:
    return_value = PyFloat_FromDouble(variant.toDouble());
    break;
  case QVariant::String:
    return_value = pystring_utf8_from_char_pointer(variant.toString().toUtf8().data());
    break;
  case QVariant::StringList:
    return_value = PyList_New(0);
    qslist = variant.toStringList();

    for (QStringList::const_iterator it = qslist.constBegin(); it != qslist.constEnd(); ++it) {
      PyList_Append(return_value, qvariant_to_pyobject(QVariant(*it)));
    }

    break;

  case QVariant::List:
    return_value = PyList_New(0);
    list = variant.toList();

    for (QVariantList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it) {
      PyList_Append(return_value, qvariant_to_pyobject(QVariant(*it)));
    }
    break;

  case QVariant::RegExp:
    return_value = PyUnicode_FromFormat("/%s/", variant.toRegExp().pattern().toUtf8().data());
    break;

  case QVariant::Map:
    return_value = PyDict_New();
    map = variant.toMap();

    for (QVariantMap::const_iterator it = map.constBegin(); it != map.constEnd(); ++it) {
      PyDict_SetItem(return_value,
                     qvariant_to_pyobject(QVariant(it.key())),
                     qvariant_to_pyobject(QVariant(*it)));
    }
    break;
  default:
    return_value = Py_None;
    Py_INCREF(return_value);
    break;
  }
  recursion_depth--;
  return return_value;
}


/* Takes a `PyObject` that must be either a `PyUNICODE*` or a
   `PyString*` and returns a UTF-8 encoded char*, and a char* with the
   source name.

   If a `PyUNICODE*` is passed it will convert it to UTF-8.  If a
   `PyString*` is passed then we assume it is already UTF-8, otherwise
   the return value will be invalid.

   If the given PyObject is not an unicode object, setting a
   TypeError exception appropriately with PyErr_SetObject.

   Return value:

   An internal reference to the char * within the UTF-8 encoded
   bytestring or `NULL` in case the given object is not an unicode
   object, so that it can be handled by the callee appropriately,
   raising the exception set with `PyErr_SetObject`.
*/
const char *
pyunicode_to_utf8_bytestring(PyObject *unicode_string, const char *source)
{
  PyObject *bytestring = NULL;
  PyObject *errorstring = NULL;

  if (!PyUnicode_Check(unicode_string) && !PyString_Check(unicode_string)) {
    errorstring = PyUnicode_FromFormat("%s takes an unicode object as parameter, got a %s instead",
                                       source,
                                       unicode_string->ob_type->tp_name);

    PyErr_SetObject(PyExc_TypeError, errorstring);
    Py_XDECREF(errorstring);
    return NULL;
  }

  if (PyUnicode_Check(unicode_string))
    bytestring = PyUnicode_AsUTF8String(unicode_string);
  else
    /* if a bytestring is passed we assume it is appropriately encoded
       as UTF-8 */
    bytestring = unicode_string;

  if (bytestring == NULL)
    return NULL;

  return PyString_AsString(bytestring);
}

/* Takes a `char *` and returns a newly allocated `PyString`
   bytestring encoded as UTF-8
*/
PyObject *
pystring_utf8_from_char_pointer(const char* data)
{
  PyObject *tmp = NULL;
  PyObject *retval = NULL;

  /* turning `char*` into a PyUnicode */
  tmp = PyUnicode_FromString(data);

  /* creating a new `PyString` from the `PyUnicode` above using the
     UTF8 codec
  */
  retval = PyUnicode_AsUTF8String(tmp);

  /* decreasing the pyunicode  reference*/
  Py_XDECREF(tmp);

  /* returning the newly allocated UTF-8 python bytestring */
  return retval;
}
