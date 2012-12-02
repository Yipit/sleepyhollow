#include <Python.h>
#include <structmember.h>

#include <yipit/hollow/hollow.h>
#include <yipit/hollow/error.h>
#include <yipit/hollow/response.h>
#include "sleepyhollow.h"

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif


/* Exceptions */

PyObject *SleepyHollowError, *InvalidUrlError;


/* The response object */

PyObject *
_response_new (int status_code, const char *text, PyTypeObject *type)
{
  SleepyHollow_Response *self;
  if ((self = (SleepyHollow_Response *) type->tp_alloc (type, 0)) == NULL)
    return NULL;

  /* For `self->json' */
  Py_INCREF (Py_None);

  self->status_code = status_code;
  self->text = PyUnicode_FromString (text);
  self->content = text;
  self->json = Py_None;
  return (PyObject *) self;
}


static PyObject *
SleepyHollow_Response_new (PyTypeObject *type,
                           PyObject *UNUSED(args),
                           PyObject *UNUSED(kwargs))
{
  PyObject *self = _response_new (0, "", type);
  return self;
}


static void
SleepyHollow_Response_dealloc (SleepyHollow_Response  *self)
{
  Py_DECREF (self->text);
  Py_DECREF (self->json);
  self->ob_type->tp_free ((PyObject *) self);
}

static struct PyMemberDef SleepyHollow_Response_members[] = {
  {(char *) "status_code", T_INT, offsetof (SleepyHollow_Response, status_code), 0,
   (char *) "The HTTP status of the request"},

  {(char *) "text", T_OBJECT, offsetof (SleepyHollow_Response, text), 0,
   (char *) "The encoded content returned from a request"},

  {(char *) "content", T_STRING, offsetof (SleepyHollow_Response, content), 0,
   (char *) "The binary content returned from a request"},

  {(char *) "json", T_OBJECT, offsetof (SleepyHollow_Response, json), 0,
   (char *) "Returns the json-encoded content of a response, if any"},

  { NULL, 0, 0, 0, 0 },         /* Sentinel */
};


static PyMethodDef SleepyHollow_Response_methods[] = {
  {NULL, NULL, 0, NULL},        /* Sentinel */
};


static PyTypeObject SleepyHollow_ResponseType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "sleepyhollow.Response",                  /* tp_name */
  sizeof (SleepyHollow_Response),           /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor) SleepyHollow_Response_dealloc, /* tp_dealloc */
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
  Py_TPFLAGS_DEFAULT,                       /* tp_flags */
  "The response for a request",             /* tp_doc */
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  0,                                        /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  SleepyHollow_Response_methods,            /* tp_methods */
  SleepyHollow_Response_members,            /* tp_members */
  0,                                        /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  0,                                        /* tp_init */
  0,                                        /* tp_alloc */
  SleepyHollow_Response_new,                /* tp_new */
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


/* Functions to help to interact with libhollow */

static PyObject *
_request_url (SleepyHollow *self, const char *method, const char *url)
{
  Response *resp;
  Error *error;

  resp = self->hollow->request (method, url);
  if ((error = Error::last()) != NULL)
    switch (error->code())
      {
      case Error::INVALID_URL:
        return PyErr_Format (InvalidUrlError, "%s\n", error->what());
      default:
        return PyErr_Format (SleepyHollowError, "Unknown Error");
      }

  return _response_new (200, // resp->getStatusCode(),
                        "Very Simple", // resp->getText().c_str(),
                        &SleepyHollow_ResponseType);
}


/* The SleepyHollow class */

static PyObject *
SleepyHollow_new (PyTypeObject *type,
                  PyObject *UNUSED(args),
                  PyObject *UNUSED(kwargs))
{
  SleepyHollow *self = NULL;

  if ((self = (SleepyHollow *) type->tp_alloc (type, 0)) == NULL)
    return NULL;
  if ((self->hollow = new Hollow) == NULL)
    {
      Py_DECREF (self);
      Py_RETURN_NONE;
    }
  return (PyObject *) self;
}


static void
SleepyHollow_dealloc (SleepyHollow  *self)
{
  delete self->hollow;
  self->ob_type->tp_free ((PyObject *) self);
}


static PyObject *
SleepyHollow_request (SleepyHollow *self, PyObject *args)
{
  const char *url, *method;
  if (!PyArg_ParseTuple (args, "ss", &method, &url))
    return NULL;
  return _request_url (self, method, url);
}


static PyObject *
SleepyHollow_get (SleepyHollow *self, PyObject *args)
{
  const char *url;
  if (!PyArg_ParseTuple (args, "s", &url))
    return NULL;
  return _request_url (self, "get", url);
}


static PyObject *
SleepyHollow_load (SleepyHollow *self, PyObject *args)
{
  char *url;
  const char *content;
  if (!PyArg_ParseTuple (args, "s", &url))
    return NULL;
  if ((content = self->hollow->getUrlContent (url)) == NULL)
    Py_RETURN_NONE;
  return PyString_FromString (content);
}


static struct PyMemberDef SleepyHollow_members[] = {
  { NULL, 0, 0, 0, 0 },   /* Sentinel */
};


static PyMethodDef SleepyHollow_methods[] = {

  {"request", (PyCFunction) SleepyHollow_request,
   METH_VARARGS, "Constructs and sends a Request. Returns Response object."},

  {"get", (PyCFunction) SleepyHollow_get,
   METH_VARARGS, "Sends a GET request. Returns Response object"},

  {"load", (PyCFunction) SleepyHollow_load,
   METH_VARARGS, "Loads a url"},

  {NULL, NULL, 0, NULL},        /* Sentinel */
};


static PyTypeObject SleepyHollowType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "sleepyhollow.SleepyHollow",              /* tp_name */
  sizeof (SleepyHollow),                    /* tp_basicsize */
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
  Py_TPFLAGS_DEFAULT,                       /* tp_flags */
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

static PyMethodDef module_methods[] = {
  {NULL, NULL, 0, NULL}         /* Sentinel */
};

PyMODINIT_FUNC
initsleepyhollow (void)
{
  PyObject *m;
  PyObject *d;

  if (PyType_Ready (&SleepyHollowType) < 0)
    return;

  if (PyType_Ready (&SleepyHollow_ResponseType) < 0)
    return;

  if ((m = Py_InitModule ("sleepyhollow", module_methods)) == NULL)
    return;

  /* Getting the module dictionary */
  if ((d = PyModule_GetDict (m)) == NULL)
    goto error;

  /* Adding our custom exceptions to the module */

  SleepyHollowError = PyErr_NewException ((char *) "sleepyhollow.Error",
                                          PyExc_StandardError, NULL);
  PyDict_SetItemString (d, (char *) "Error", SleepyHollowError);

  InvalidUrlError = PyErr_NewException ((char *) "sleepyhollow.InvalidUrlError",
                                        SleepyHollowError, NULL);
  PyDict_SetItemString (d, (char *)"InvalidUrlError", InvalidUrlError);

  /* Adding the classes */

  Py_INCREF (&SleepyHollowType);
  PyModule_AddObject (m, "SleepyHollow", (PyObject *) &SleepyHollowType);

  Py_INCREF (&SleepyHollow_ResponseType);
  PyModule_AddObject (m, "Response", (PyObject *) &SleepyHollow_ResponseType);

  /* Error Handling */
 error:
  if (PyErr_Occurred ())
    PyErr_SetString (PyExc_ImportError, "sleepyhollow: init failed");
}
