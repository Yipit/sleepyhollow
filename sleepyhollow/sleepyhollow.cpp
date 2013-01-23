#include <Python.h>
#include <structmember.h>

#include <hollow/hollow.h>
#include <hollow/jserror.h>
#include <hollow/error.h>
#include <hollow/response.h>
#include "sleepyhollow.h"

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#define C_STR(x) ((char *) x)

/* using recursion depth to avoid evaluating deserializing js objects
   that are too big */
#define HOLLOW_MAX_RECURSION_DEPTH 50
static int recursion_depth = 0;

/* Exceptions */

PyObject *SleepyHollowError, *InvalidUrlError, *ConnectionRefusedError, *BadCredentialsError, *InvalidJSONError;

/* Helpers */

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
prepare_sleepy_hollow_response(Response* response)
{
  /*
    Takes a Response instance and takes care of turning it into a
    dictionary with the response that will be handled by the python
    wrapper.
   */

  PyObject *dict;
  dict = PyDict_New();

  PyDict_SetItemString(dict, C_STR("url"),
                       PyUnicode_FromString(response->getURL()));
  PyDict_SetItemString(dict, C_STR("text"),
                       PyUnicode_FromString(response->getText()));
  PyDict_SetItemString(dict, C_STR("html"),
                       PyUnicode_FromString(response->getHtml()));

  Error *error;
  int status_code;
  const char *reason;

  error = Error::last();
  if (error != NULL && error->code() == Error::BAD_CREDENTIALS) {
    status_code = 401;
    reason = "UNAUTHORIZED";
  } else {
    status_code = response->getStatusCode();
    reason = response->getReason();
  }

  PyDict_SetItemString(dict, C_STR("status_code"),
                       PyInt_FromLong(status_code));
  PyDict_SetItemString(dict, C_STR("reason"),
                       PyString_FromString(reason));

  PyDict_SetItemString(dict, C_STR("screenshot_bytes_base64"),
                       PyString_FromString(response->getScreenshotData()));

  /* Adding the headers */
  PyObject *response_headers_dict =
    string_hash_map_to_pydict(response->getHeaders());
  PyDict_SetItemString(dict, C_STR("headers"), response_headers_dict);

  /* Adding the js errors */
  PyObject *errors_tuple =
    js_error_list_to_pytuple(response->getJSErrors());
  PyDict_SetItemString(dict, C_STR("js_errors"), errors_tuple);

  /* Adding the list of requested resources */
  PyObject *resources =
    stringlist_to_python_tuple(response->getRequestedResources());
  PyDict_SetItemString(dict, C_STR("requested_resources"), resources);

  return dict;
}

/* The SleepyHollow class */

static PyObject *
SleepyHollow_new(PyTypeObject *type, PyObject *UNUSED(args), PyObject *UNUSED(kwargs))
{
  SleepyHollow *self = NULL;

  if ((self = (SleepyHollow *) type->tp_alloc(type, 0)) == NULL)
    return NULL;
  if ((self->hollow = new Hollow(0)) == NULL) {
    Py_DECREF(self);
    Py_RETURN_NONE;
  }
  return (PyObject *) self;
}

static void
SleepyHollow_dealloc(SleepyHollow *self)
{
  delete self->hollow;
  self->ob_type->tp_free((PyObject *) self);
}

static PyObject *
SleepyHollow_request(SleepyHollow *self, PyObject *args, PyObject *kw)
{
  Response *resp;
  Error *error;
  PyObject *payload_str = NULL;
  PyObject *request_headers_dict = NULL;
  PyObject *auth_tuple = NULL;
  PyObject *config_dict = NULL;
  char *payload = NULL;
  const char *url, *method;
  static char *kwlist[] = {
    C_STR("method"),
    C_STR("url"),
    C_STR("params"),
    C_STR("headers"),
    C_STR("auth"),
    C_STR("config"),
    NULL
  };

  if (!PyArg_ParseTupleAndKeywords(args, kw, "ss|OOOO", kwlist, &method, &url,
                                   &payload_str, &request_headers_dict,
                                   &auth_tuple, &config_dict))
    return NULL;

  /* If params is not PyString or Py_None we raise a TypeError */
  if (PyString_Check(payload_str))
    payload = PyString_AsString(payload_str);
  else if (payload_str != Py_None)
    return PyErr_Format(PyExc_TypeError,
                        "The 'params' argument must be either a string or None");

  /* Checking if headers is a dict, and if so, we turn it into a StringHashMap */
  StringHashMap requestHeaders;
  if (PyDict_Check(request_headers_dict))
    requestHeaders = pydict_to_string_hash_map(request_headers_dict);
  else if (request_headers_dict != Py_None)
    return PyErr_Format(PyExc_TypeError,
                        "The 'headers' argument must be either a dict or None");

  /* Creating the Config object based on the dict we received from the
   * python side of the force */
  Config config;
  if (config_dict && PyDict_Check(config_dict))
    config = pydict_to_config(config_dict);
  else if (config_dict != NULL && config_dict != Py_None)
    return PyErr_Format(PyExc_TypeError,
                        "The 'config' argument must be either a dict or None");

  /*  */
  UsernamePasswordPair credentials;
  if (auth_tuple && PyTuple_Check(auth_tuple)) {
    int tuple_size = PyTuple_Size(auth_tuple);
    if (tuple_size == 0 || tuple_size == 2)
      credentials = pytuple_to_credentials(auth_tuple, tuple_size);
    else
      return PyErr_Format(PyExc_TypeError,
                          "The 'auth' tuple should have either 0 or 2 elements");

  } else if (auth_tuple != NULL)
    return PyErr_Format(PyExc_TypeError,
                        "The 'auth' argument must be a tuple");

  /* Performing the actuall request */
  resp = self->hollow->request(method, url, payload, requestHeaders, credentials, config);

  /* Just making sure that everything worked */
  error = Error::last();
  if (resp == NULL && error == NULL)
    return PyErr_Format(SleepyHollowError, "Both error and response are null");
  else if (error != NULL)
    switch (error->code()) {
      case Error::INVALID_URL:
        return PyErr_Format(InvalidUrlError, "%s", error->what());
      case Error::CONNECTION_REFUSED:
        return PyErr_Format(ConnectionRefusedError, "%s", error->what());
      case Error::USER_ABORTED:
        return PyErr_Format(PyExc_KeyboardInterrupt, "%s", error->what());
      default:
        if (credentials.first.length() > 0 || credentials.second.length() > 0)
          return PyErr_Format(BadCredentialsError, "Either username or password are wrong");
        else
          return PyErr_Format(SleepyHollowError, "Unknown Error");
      }

  /* Returning a dictionary with the values grabbed from the above
   * request call */
  return prepare_sleepy_hollow_response(resp);
}

static PyObject *
SleepyHollow_deserialize_qvariant(QVariant variant)
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
    return_value = PyUnicode_FromString(variant.toString().toUtf8().data());
    break;
  case QVariant::StringList:
    return_value = PyList_New(0);
    qslist = variant.toStringList();

    for (QStringList::const_iterator it = qslist.constBegin(); it != qslist.constEnd(); ++it) {
      PyList_Append(return_value, SleepyHollow_deserialize_qvariant(QVariant(*it)));
    }

    break;

  case QVariant::List:
    return_value = PyList_New(0);
    list = variant.toList();

    for (QVariantList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it) {
      PyList_Append(return_value, SleepyHollow_deserialize_qvariant(QVariant(*it)));
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
                     SleepyHollow_deserialize_qvariant(QVariant(it.key())),
                     SleepyHollow_deserialize_qvariant(QVariant(*it)));
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

static PyObject *
SleepyHollow_evaluate_javascript(SleepyHollow *self, PyObject *args, PyObject *kw)
{
  Error *error = NULL;

  const char *script = NULL;
  PyObject *unicode_script = NULL;
  PyObject *utf8bytes_script = NULL;

  static char *kwlist[] = {
    C_STR("script"),
    NULL
  };

  if (!PyArg_ParseTupleAndKeywords(args, kw, "O", kwlist, &unicode_script))
    return NULL;

  if (PyString_Check(unicode_script))
    return PyErr_Format(PyExc_TypeError,
                        "SleepyHollow.evaluate_javascript "
                        "takes an unicode object as parameter, "
                        "got a bytestring instead");

  utf8bytes_script = PyUnicode_AsUTF8String(unicode_script);
  if (utf8bytes_script == NULL)
    return PyErr_Format(PyExc_ValueError,
                        "The given javascript cannot be encoded as utf-8");

  script = PyString_AsString(utf8bytes_script);
  QVariant variant = self->hollow->evaluateJavaScript(script);

  error = Error::last();
  if (error != NULL)
    return PyErr_Format(InvalidJSONError, "Invalid JSON: %s", error->what());

  return SleepyHollow_deserialize_qvariant(variant);
}

static struct PyMemberDef SleepyHollow_members[] = {
  { NULL, 0, 0, 0, 0 },   /* Sentinel */
};

static PyMethodDef SleepyHollow_methods[] = {

  {"request", (PyCFunction) SleepyHollow_request,
   METH_VARARGS | METH_KEYWORDS, "Constructs and sends a Request. Returns Response object."},
  {"evaluate_javascript", (PyCFunction) SleepyHollow_evaluate_javascript,
   METH_VARARGS | METH_KEYWORDS, "Evaluates the given string as javascript and returns a json serialized string"},
  {NULL, NULL, 0, NULL},        /* Sentinel */
};


static PyTypeObject SleepyHollowType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "_sleepyhollow.SleepyHollow",             /* tp_name */
  sizeof(SleepyHollow),                     /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor) SleepyHollow_dealloc,        /* tp_dealloc */
  0,                                        /* tp_print */
  0,                                        /* tp_getattr */
  0,                                        /* tp_setattr */
  0,                                        /* tp_compare */
  0,                                        /* tp_repr */
  0,                                        /* tp_as_number */
  0,                                        /* tp_as_sequence */
  0,                                        /* tp_as_mapping */
  0,                                        /* tp_hash */
  0,                                        /* tp_call */
  0,                                        /* tp_str */
  0,                                        /* tp_getattro */
  0,                                        /* tp_setattro */
  0,                                        /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
  "SleepyHollow is an amazing scrapper",    /* tp_doc */
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  0,                                        /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  SleepyHollow_methods,                     /* tp_methods */
  SleepyHollow_members,                     /* tp_members */
  0,                                        /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  0,                                        /* tp_init */
  0,                                        /* tp_alloc */
  SleepyHollow_new,                         /* tp_new */
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};


/* The module definition */

PyObject *
SleepyHollow_setup(PyObject *UNUSED(self), PyObject *UNUSED(args))
{
  Hollow::setup();
  Py_RETURN_NONE;
}

PyObject *
SleepyHollow_teardown(PyObject *UNUSED(self), PyObject *UNUSED(args))
{
  Hollow::teardown();
  Py_RETURN_NONE;
}

static PyMethodDef module_methods[] = {
  {"setup",  SleepyHollow_setup, METH_NOARGS,
   "Initializes the QApplication instance needed to run sleepy hollow"},
  {"teardown",  SleepyHollow_teardown, METH_NOARGS,
   "Deinitializes the QApplication instance attached to sleepy hollow."},
  {NULL, NULL, 0, NULL}         /* Sentinel */
};

PyMODINIT_FUNC
init_sleepyhollow (void)
{
  PyObject *m;
  PyObject *d;

  if (PyType_Ready(&SleepyHollowType) < 0)
    return;

  if ((m = Py_InitModule("_sleepyhollow", module_methods)) == NULL)
    return;

  /* Getting the module dictionary */
  if ((d = PyModule_GetDict(m)) == NULL)
    goto error;

  /* Adding the QT_VERSION constant */
  PyDict_SetItemString(d,
                       C_STR("QT_VERSION"),
                       PyInt_FromLong(Hollow::QT_VERSION_NUMBER));

  /* Adding our custom exceptions to the module */

  SleepyHollowError =
    PyErr_NewException(C_STR("sleepyhollow.Error"),
                       PyExc_StandardError, NULL);
  PyDict_SetItemString(d,
                       C_STR("Error"),
                       SleepyHollowError);

  BadCredentialsError =
    PyErr_NewException(C_STR("sleepyhollow.BadCredentialsError"),
                       PyExc_StandardError, NULL);
  PyDict_SetItemString(d,
                       C_STR("BadCredentialsError"),
                       BadCredentialsError);

  InvalidUrlError =
    PyErr_NewException(C_STR("sleepyhollow.InvalidUrlError"),
                       SleepyHollowError, NULL);
  PyDict_SetItemString(d,
                       C_STR("InvalidUrlError"),
                       InvalidUrlError);

  ConnectionRefusedError =
    PyErr_NewException(C_STR ("sleepyhollow.ConnectionRefusedError"),
                       SleepyHollowError, NULL);
  PyDict_SetItemString(d,
                       C_STR("ConnectionRefusedError"),
                       ConnectionRefusedError);


  InvalidJSONError =
    PyErr_NewException(C_STR ("sleepyhollow.InvalidJSONError"),
                       SleepyHollowError, NULL);
  PyDict_SetItemString(d,
                       C_STR("InvalidJSONError"),
                       InvalidJSONError);

  /* Adding the classes */

  Py_INCREF(&SleepyHollowType);
  PyModule_AddObject(m, "SleepyHollow", (PyObject *) &SleepyHollowType);

  /* Error Handling */
 error:
  if (PyErr_Occurred())
    PyErr_SetString(PyExc_ImportError, "_sleepyhollow: init failed");
}
