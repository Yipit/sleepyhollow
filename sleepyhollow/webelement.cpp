#include <Python.h>
#include <structmember.h>
#include <hollow/hollow.h>


#include "helpers.h"
#include "webelement.h"

static PyObject *
WebElement_new(PyTypeObject *type, PyObject *args, PyObject *kw)
{
  WebElement *self = NULL;

  if ((self = (WebElement *) type->tp_alloc(type, 0)) == NULL)
    return NULL;

  static char *kwlist[] = {
    NULL
  };

  if (!PyArg_ParseTupleAndKeywords(args, kw, "", kwlist))
    return NULL;

  return (PyObject *) self;
}

static void
WebElement_dealloc(WebElement *self)
{
  self->ob_type->tp_free((PyObject *) self);
}

static PyObject *
WebElement_get_attribute(WebElement *self, PyObject *args, PyObject *kw)
{
  char *name = NULL;
  PyObject *fallback = NULL;

  static char *kwlist[] = {
    C_STR("name"),
    C_STR("fallback"),
    NULL
  };

  if (!PyArg_ParseTupleAndKeywords(args, kw, "s|O", kwlist, &name, &fallback))
    return NULL;

  if ((fallback != NULL) && !PyString_Check(fallback) && (fallback != Py_None))
    return PyErr_Format(PyExc_TypeError,
                        "The 'fallback' argument must be either a string or None");

  if (fallback == NULL) {
    return PyString_FromString(self->element->attribute(QString(name)).toStdString().data());
  }
  return PyString_FromString(self->element->attribute(QString(name), QString(fallback == Py_None ? "": PyString_AsString(fallback))).toStdString().data());
}

static PyObject *
WebElement_get_text(WebElement *self, PyObject *UNUSED(args), PyObject *UNUSED(kw))
{
  return PyString_FromString(self->element->toPlainText().toStdString().data());
}

static PyObject *
WebElement_get_outer_html(WebElement *self, PyObject *UNUSED(args), PyObject *UNUSED(kw))
{
  return PyString_FromString(self->element->toOuterXml().toStdString().data());
}

static PyObject *
WebElement_get_inner_html(WebElement *self, PyObject *UNUSED(args), PyObject *UNUSED(kw))
{
  return PyString_FromString(self->element->toInnerXml().toStdString().data());
}

static PyObject *
WebElement_evaluate_javascript(WebElement *self, PyObject *args, PyObject *kw)
{
  const char *script = NULL;
  static char *kwlist[] = {
    C_STR("script"),
    NULL
  };

  if (!PyArg_ParseTupleAndKeywords(args, kw, "s", kwlist, &script))
    return NULL;

  QVariant variant = self->element->evaluateJavaScript(QString(script));
  return qvariant_to_pyobject(variant);
}

static PyObject *
WebElement_get_attribute_names(WebElement *self, PyObject *args, PyObject *kw)
{
  PyObject *list = NULL;
  PyObject *ns = NULL;
  static char *kwlist[] = {
    C_STR("namespace"),
    NULL
  };

  if (!PyArg_ParseTupleAndKeywords(args, kw, "|O", kwlist, &ns))
    return NULL;

  if ((ns != NULL) && !PyString_Check(ns) && ns != Py_None)
    return PyErr_Format(PyExc_TypeError, "Invalid type for `namespace`. It must be either a string or None");

  QStringList attrs;
  if (ns == NULL || ns == Py_None) {
    attrs = self->element->attributeNames();
  } else {
    self->element->attributeNames(QString(PyString_AsString(ns)));
  }

  list = PyList_New(0);

  foreach (const QString &str, attrs) {
    PyList_Append(list, PyString_FromString(str.toStdString().data()));
  }
  return list;
}

static PyObject *
WebElement_cssselect(WebElement *self, PyObject *args, PyObject *kw)
{
  const char *selector = NULL;
  static char *kwlist[] = {
    C_STR("selector"),
    NULL
  };

  if (!PyArg_ParseTupleAndKeywords(args, kw, "s", kwlist, &selector))
    return NULL;

  PyObject *list = NULL;
  PyObject *element = NULL;
  QList<QWebElement> elements = self->element->findAll(selector).toList();

  int total_elements = elements.length();
  list = PyList_New(0);

  for (int i = 0; i < total_elements; ++i) {
    element = webelement_from_hollow_and_qwebelement(self->hollow, elements.at(i));
    PyList_Append(list, element);
  }

  return list;
}

static PyObject *
WebElement_repr(PyObject *_self)
{
  WebElement *self = NULL;
  self = (WebElement *)_self;
  return PyString_FromFormat("<WebElement tag='%s'>", self->element->tagName().toLower().toStdString().data());
}


static PyMethodDef WebElement_methods[] = {
  {"cssselect", (PyCFunction) WebElement_cssselect,
   METH_VARARGS | METH_KEYWORDS, "Queries for elements"},
  {"get_attribute", (PyCFunction) WebElement_get_attribute,
   METH_VARARGS | METH_KEYWORDS, "returns an attribute or its fallback"},
  {"get_text", (PyCFunction) WebElement_get_text,
   METH_VARARGS | METH_KEYWORDS, "returns the inner text"},
  {"get_outer_html", (PyCFunction) WebElement_get_outer_html,
   METH_VARARGS | METH_KEYWORDS, "returns the inner html"},
  {"get_inner_html", (PyCFunction) WebElement_get_inner_html,
   METH_VARARGS | METH_KEYWORDS, "returns the inner html"},
  {"evaluate_javascript", (PyCFunction) WebElement_evaluate_javascript,
   METH_VARARGS | METH_KEYWORDS, "Evaluates the given string as javascript and returns a json serialized string"},
  {"get_attribute_names", (PyCFunction) WebElement_get_attribute_names,
   METH_VARARGS | METH_KEYWORDS, "Returns all the attribute names of this element"},
  {NULL, NULL, 0, NULL},        /* Sentinel */
};

static struct PyMemberDef WebElement_members[] = {
  { NULL, 0, 0, 0, 0 },   /* Sentinel */
};



static PyTypeObject WebElementType = {
  PyObject_HEAD_INIT(NULL)
  0,                                        /* ob_size */
  "_sleepyhollow.WebElement",             /* tp_name */
  sizeof(WebElement),                     /* tp_basicsize */
  0,                                        /* tp_itemsize */
  (destructor) WebElement_dealloc,        /* tp_dealloc */
  0,                                        /* tp_print */
  0,                                        /* tp_getattr */
  0,                                        /* tp_setattr */
  0,                                        /* tp_compare */
  WebElement_repr,                          /* tp_repr */
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
  "WebElement is a DOM element acessible by python",    /* tp_doc */
  0,                                        /* tp_traverse */
  0,                                        /* tp_clear */
  0,                                        /* tp_richcompare */
  0,                                        /* tp_weaklistoffset */
  0,                                        /* tp_iter */
  0,                                        /* tp_iternext */
  WebElement_methods,                     /* tp_methods */
  WebElement_members,                     /* tp_members */
  0,                                        /* tp_getset */
  0,                                        /* tp_base */
  0,                                        /* tp_dict */
  0,                                        /* tp_descr_get */
  0,                                        /* tp_descr_set */
  0,                                        /* tp_dictoffset */
  0,                                        /* tp_init */
  0,                                        /* tp_alloc */
  WebElement_new,                         /* tp_new */
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
