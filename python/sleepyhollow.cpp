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

#define C_STR(x) ((char *) x)

/* Exceptions */

PyObject *SleepyHollowError, *InvalidUrlError, *ConnectionRefusedError;

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
  PyObject *dict;
  Response *resp;
  Error *error;
  const char *url, *method;

  if (!PyArg_ParseTuple (args, "ss", &method, &url))
    return NULL;

  resp = self->hollow->request (method, url);

  /* Just making sure that everything worked */
  if ((error = Error::last()) != NULL)
    switch (error->code())
      {
      case Error::INVALID_URL:
        return PyErr_Format (InvalidUrlError, "%s", error->what());
      case Error::CONNECTION_REFUSED:
        return PyErr_Format (ConnectionRefusedError, "%s", error->what());
      default:
        return PyErr_Format (SleepyHollowError, "Unknown Error");
      }

  /* Returning a dictionary with the values grabbed from the above
   * request call */
  if ((dict = PyDict_New ()) == NULL)
    return NULL;

  PyDict_SetItemString (dict, C_STR ("text"),
                        PyUnicode_FromString (resp->getText()));
  PyDict_SetItemString (dict, C_STR ("status_code"),
                        PyInt_FromLong (resp->getStatusCode()));
  PyDict_SetItemString (dict, C_STR ("reason"),
                        PyString_FromString (resp->getReason()));

  /* Adding the headers */
  ResponseHeaders headers = resp->getHeaders();;
  ResponseHeadersIterator iterator;
  PyObject *headers_dict = PyDict_New ();

  PyDict_SetItemString (dict, C_STR ("headers"), headers_dict);
  for (iterator = headers.begin(); iterator != headers.end(); iterator++)
    PyDict_SetItemString (headers_dict, C_STR (iterator->first.c_str()),
                          PyString_FromString (iterator->second.c_str()));
  return dict;
}


static struct PyMemberDef SleepyHollow_members[] = {
  { NULL, 0, 0, 0, 0 },   /* Sentinel */
};


static PyMethodDef SleepyHollow_methods[] = {

  {"request", (PyCFunction) SleepyHollow_request,
   METH_VARARGS, "Constructs and sends a Request. Returns Response object."},

  {NULL, NULL, 0, NULL},        /* Sentinel */
};


static PyTypeObject SleepyHollowType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "_sleepyhollow.SleepyHollow",             /* tp_name */
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

static PyMethodDef module_methods[] = {
  {NULL, NULL, 0, NULL}         /* Sentinel */
};

PyMODINIT_FUNC
init_sleepyhollow (void)
{
  PyObject *m;
  PyObject *d;

  if (PyType_Ready (&SleepyHollowType) < 0)
    return;

  if ((m = Py_InitModule ("_sleepyhollow", module_methods)) == NULL)
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
  PyDict_SetItemString (d, (char *) "InvalidUrlError", InvalidUrlError);

  ConnectionRefusedError = PyErr_NewException ((char *) "sleepyhollow.ConnectionRefusedError",
                                               SleepyHollowError, NULL);
  PyDict_SetItemString (d, (char *) "ConnectionRefusedError", ConnectionRefusedError);

  /* Adding the classes */

  Py_INCREF (&SleepyHollowType);
  PyModule_AddObject (m, "SleepyHollow", (PyObject *) &SleepyHollowType);

  /* Error Handling */
 error:
  if (PyErr_Occurred ())
    PyErr_SetString (PyExc_ImportError, "_sleepyhollow: init failed");
}
