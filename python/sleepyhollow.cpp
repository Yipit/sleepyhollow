#include <Python.h>
#include <structmember.h>

#include <yipit/hollow/hollow.h>
#include "sleepyhollow.h"

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

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
                           PyObject *args,
                           PyObject *kwargs)
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
  {"status_code", T_INT, offsetof (SleepyHollow_Response, status_code), 0,
   "The HTTP status of the request"},

  {"text", T_OBJECT, offsetof (SleepyHollow_Response, text), 0,
   "The encoded content returned from a request"},

  {"content", T_STRING, offsetof (SleepyHollow_Response, content), 0,
   "The binary content returned from a request"},

  {"json", T_OBJECT, offsetof (SleepyHollow_Response, json), 0,
   "Returns the json-encoded content of a response, if any"},

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
  char *url, *method;
  PyObject *response;
  if (!PyArg_ParseTuple (args, "ss", &method, &url))
    return NULL;
  return _response_new (200, "Very Simple", &SleepyHollow_ResponseType);
}


static PyObject *
SleepyHollow_get (SleepyHollow *self, PyObject *args)
{
  char *url;
  PyObject *response;
  if (!PyArg_ParseTuple (args, "s", &url))
    return NULL;
  return _response_new (200, "Very Simple", &SleepyHollow_ResponseType);
}


static PyObject *
SleepyHollow_load (SleepyHollow *self, PyObject *args)
{
  char *url;
  if (!PyArg_ParseTuple (args, "s", &url))
    return NULL;
  try
    {
      const char *content = self->hollow->getUrlContent (url);
      return PyString_FromString (content);
    }
  catch (std::exception &exc)
    {
      Py_INCREF (Py_None);
      return Py_None;
    }
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

  if (PyType_Ready (&SleepyHollowType) < 0)
    return;

  if (PyType_Ready (&SleepyHollow_ResponseType) < 0)
    return;

  if ((m = Py_InitModule ("sleepyhollow", module_methods)) == NULL)
    return;

  Py_INCREF (&SleepyHollowType);
  PyModule_AddObject (m, "SleepyHollow", (PyObject *) &SleepyHollowType);

  Py_INCREF (&SleepyHollow_ResponseType);
  PyModule_AddObject (m, "Response", (PyObject *) &SleepyHollow_ResponseType);
}
