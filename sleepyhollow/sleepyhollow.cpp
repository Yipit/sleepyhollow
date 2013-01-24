#include <Python.h>
#include <structmember.h>

#include <hollow/hollow.h>
#include <hollow/jserror.h>
#include <hollow/error.h>
#include <hollow/response.h>

#include "core.h"
#include "sleepyhollow.h"

#include "webelement.cpp"
#include "helpers.cpp"


/* Exceptions */

PyObject *SleepyHollowError, *InvalidUrlError, *ConnectionRefusedError, *BadCredentialsError, *InvalidJSONError;

/* Helpers */

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
SleepyHollow_evaluate_javascript(SleepyHollow *self, PyObject *args, PyObject *kw)
{
  Error *error = NULL;

  const char *script = NULL;
  PyObject *unicode_script = NULL;
  PyObject *pedantic = NULL;
  int should_be_pedantic = 0;
  static char *kwlist[] = {
    C_STR("script"),
    C_STR("pedantic"),
    NULL
  };

  if (!PyArg_ParseTupleAndKeywords(args, kw, "O|O", kwlist, &unicode_script, &pedantic))
    return NULL;

  script = pyunicode_to_utf8_bytestring(unicode_script,
                                        "SleepyHollow.evaluate_javascript");
  if (script == NULL)
    return NULL;

  should_be_pedantic = pedantic != NULL && PyBool_Check(pedantic) && pedantic == Py_True;

  QVariant variant = self->hollow->evaluateJavaScript(script);

  error = Error::last();
  if (should_be_pedantic && (error != NULL))
    return PyErr_Format(InvalidJSONError, "Invalid JSON: %s", error->what());

  return qvariant_to_pyobject(variant);
}

static PyObject *
SleepyHollow_get_dom(SleepyHollow *self)
{
  QWebElement element = self->hollow->document();
  return webelement_from_hollow_and_qwebelement(self->hollow, element);
}
static PyObject *
SleepyHollow_get_response(SleepyHollow *self)
{

  return prepare_sleepy_hollow_response(self->hollow->lastResponse());
}

static struct PyMemberDef SleepyHollow_members[] = {
  { NULL, 0, 0, 0, 0 },   /* Sentinel */
};

static PyMethodDef SleepyHollow_methods[] = {

  {"request", (PyCFunction) SleepyHollow_request,
   METH_VARARGS | METH_KEYWORDS, "Constructs and sends a Request. Returns Response object."},
  {"evaluate_javascript", (PyCFunction) SleepyHollow_evaluate_javascript,
   METH_VARARGS | METH_KEYWORDS, "Evaluates the given string as javascript and returns a json serialized string"},
  {"get_dom", (PyCFunction) SleepyHollow_get_dom,
   METH_VARARGS | METH_KEYWORDS, "Returns the root element for the document"},
  {"get_response", (PyCFunction) SleepyHollow_get_response,
   METH_VARARGS | METH_KEYWORDS, "Returns the last response data"},
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

  if (PyType_Ready(&WebElementType) < 0)
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

  Py_INCREF(&WebElementType);
  PyModule_AddObject(m, "WebElement", (PyObject *) &WebElementType);

  /* Error Handling */
 error:
  if (PyErr_Occurred())
    PyErr_SetString(PyExc_ImportError, "_sleepyhollow: init failed");
}
