#include <Python.h>
#include <structmember.h>

#include <yipit/hollow/hollow.h>

typedef struct {
  PyObject_HEAD;
  y_hollow_t *hollow;
} SleepyHollow;


static void
SleepyHollow_dealloc (SleepyHollow  *self)
{
  self->ob_type->tp_free ((PyObject *) self);
}


static PyObject *
SleepyHollow_load (SleepyHollow *self)
{
  Py_INCREF (Py_None);
  return Py_None;
}


static struct PyMemberDef SleepyHollow_members[] = {
  { NULL, 0, 0, 0 },   /* Sentinel */
};


static PyMethodDef SleepyHollow_methods[] = {

  {"load", (PyCFunction) SleepyHollow_load,
   METH_VARARGS, "Loads a url"},

  {NULL, NULL, 0, NULL},        /* Sentinel */
};


static PyTypeObject SleepyHollowType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "sleepy_hollow.SleepyHollow",             /* tp_name */
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
  "SleepyHollow Objects",                   /* tp_doc */
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
  PyType_GenericNew,                        /* tp_new */
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

static PyMethodDef module_methods[] = {
  {NULL, NULL, 0, NULL}         /* Sentinel */
};

PyMODINIT_FUNC
initsleepy_hollow (void)
{
  PyObject *m;

  if (PyType_Ready (&SleepyHollowType) < 0)
    return;

  if ((m = Py_InitModule ("sleepy_hollow", module_methods)) == NULL)
    return;

  Py_INCREF (&SleepyHollowType);
  PyModule_AddObject (m, "SleepyHollow", (PyObject *) &SleepyHollowType);
}
