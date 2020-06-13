#include <Noon/Script.hpp>

#include <Python.h>
#include <frameobject.h>

#include <SFML/System.hpp>
using sf::Vector2f;
using sf::Vector2i;
using sf::Vector2u;

#include "Engine.gen.hpp"

static PyObject * noon_info(PyObject * self, PyObject * args) {
    UNUSED(self);

    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        return nullptr;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    printf("INFO[%d]: %s\n", line, msg);
    Py_RETURN_NONE;
}

static PyObject * noon_warn(PyObject * self, PyObject * args) {
    UNUSED(self);
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        return nullptr;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    fprintf(stderr, "WARN[%d]: %s\n", line, msg);
    Py_RETURN_NONE;
}

static PyObject * noon_error(PyObject * self, PyObject * args) {
    UNUSED(self);
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        return nullptr;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    fprintf(stderr, "ERRO[%d]: %s\n", line, msg);
    Py_RETURN_NONE;
}

static PyMethodDef NoonMethods[] = {
    {"info",  noon_info, METH_VARARGS, nullptr},
    {"warn",  noon_warn, METH_VARARGS, nullptr},
    {"error",  noon_error, METH_VARARGS, nullptr},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef NoonModule = {
    PyModuleDef_HEAD_INIT,
    "noon",             // m_name
    nullptr,            // m_doc
    -1,                 // m_size
    NoonMethods,        // m_methods
    nullptr,            // m_slots
    nullptr,            // m_traverse
    nullptr,            // m_clear
    nullptr,            // m_free
};

PyMODINIT_FUNC PyInit_noon()
{
    PyObject * module = nullptr;

    module = PyModule_Create(&NoonModule);
    if (!module) {
        return nullptr;
    }

    _Script_InitGen(module);

    return module;
}

void PrintStackTrace()
{
    if (PyErr_Occurred()) {
        PyObject * pyType = nullptr;
        PyObject * pyValue = nullptr;
        PyObject * pyTrace = nullptr;

        PyErr_Fetch(&pyType, &pyValue, &pyTrace);
        PyErr_NormalizeException(&pyType, &pyValue, &pyTrace);

        PyObject * pyValueRepr = PyObject_Repr(pyValue);
        if (pyValueRepr) {
            PyObject * pyValueStr = PyUnicode_AsEncodedString(pyValueRepr, "utf-8", "~E~");

            fprintf(stderr,
                "[ERRO] Exception %s\n", 
                PyBytes_AS_STRING(pyValueStr));

            Py_XDECREF(pyValueStr);
            Py_XDECREF(pyValueRepr);
        }
        else {
            PyObject * pyTypeName = PyObject_GetAttrString(pyType, "__name__");
            PyObject * pyTypeNameStr = PyUnicode_AsEncodedString(pyTypeName, "utf-8", "~E~");

            fprintf(stderr,
                "[ERRO] Exception %s\n", 
                PyBytes_AS_STRING(pyTypeNameStr));

            Py_XDECREF(pyTypeNameStr);
            Py_XDECREF(pyTypeName);
        }
    }
}

bool InitScript()
{
    PyImport_AppendInittab("noon", PyInit_noon);
    Py_Initialize();
    PyImport_ImportModule("noon");

    return true;
}

void TermScript()
{
    Py_Finalize();
}