#include "ScriptHost.hpp"
#include <App.hpp>
#include <Actor.hpp>

#include <Python.h>
#include <frameobject.h>

#include <fstream>
using std::ifstream;
using std::istreambuf_iterator;

#include <SFML/System.hpp>
using sf::Vector2f;
using sf::Vector2i;
using sf::Vector2u;

static PyObject * noon_info(PyObject * self, PyObject * args) {
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
    const char * msg;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        return nullptr;
    }

    PyFrameObject * frame = PyEval_GetFrame();
    int line = PyFrame_GetLineNumber(frame);

    fprintf(stderr, "ERRO[%d]: %s\n", line, msg);
    Py_RETURN_NONE;
}

struct ActorType {
    PyObject_HEAD
    Actor * _ptr;
};

static PyObject * noon_Actor_get_name(PyObject * self, PyObject * args) {
    Actor * actor = ((ActorType *)self)->_ptr;
    return PyUnicode_FromString(actor->GetName().c_str());
}

static PyMethodDef ActorTypeMethods[] = {
    {"get_name",  noon_Actor_get_name, METH_NOARGS, nullptr},
    {NULL}
};

static PyTypeObject noon_tp_Actor = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "noon.Actor",                             /* tp_name */
    sizeof(ActorType),                        /* tp_basicsize */
    0,                                        /*tp_itemsize*/
    0,                                        /* tp_dealloc */
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
    0,                                        /* tp_doc */
    0,                                        /* tp_traverse */
    0,                                        /* tp_clear */
    0,                                        /* tp_richcompare */
    0,                                        /* tp_weaklistoffset */
    0,                                        /* tp_iter */
    0,                                        /* tp_iternext */
    ActorTypeMethods,                         /* tp_methods */
    0,                                        /* tp_members */
    0,                                        /* tp_getset */
    0,                                        /* tp_base */
    0,                                        /* tp_dict */
    0,                                        /* tp_descr_get */
    0,                                        /* tp_descr_set */
    0,                                        /* tp_dictoffset */
    0,                                        /* tp_init */
    0,                                        /* tp_alloc */
    0, // filled in with type_new             /* tp_new */
    0, // filled in with __PyObject_GC_Del    /* tp_free */
    0,                                        /* tp_is_gc */
    0,                                        /* tp_bases */
    0,                                        /* tp_mro */
    0,                                        /* tp_cache */
    0,                                        /* tp_subclasses */
    0,                                        /* tp_weaklist */
#if PYTHON_API_VERSION >= 1012
    0                                         /* tp_del */
#endif
};

static PyMethodDef NoonMethods[] = {
    {"info",  noon_info, METH_VARARGS, nullptr},
    {"warn",  noon_warn, METH_VARARGS, nullptr},
    {"error",  noon_error, METH_VARARGS, nullptr},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef NoonModule = {
    PyModuleDef_HEAD_INIT,
    "noon",
    nullptr,
    -1,
    NoonMethods
};

PyMODINIT_FUNC PyInit_noon() {
    PyObject * mod;

    noon_tp_Actor.tp_new = PyType_GenericNew;
    if (PyType_Ready(&noon_tp_Actor) < 0) {
        return nullptr;
    }

    mod = PyModule_Create(&NoonModule);
    if (!mod) {
        return nullptr;
    }

    Py_INCREF(&noon_tp_Actor);
    PyModule_AddObject(mod, "Actor", (PyObject *)&noon_tp_Actor);

    return mod;
}

void ScriptHost::SetOwner(Actor * actor) {
    ActorType * obj = (ActorType *)PyType_GenericNew(&noon_tp_Actor, NULL, NULL);
    obj->_ptr = actor;
    Py_INCREF(obj);

    PyObject * mod = PyImport_GetModule(PyUnicode_FromString("__main__"));
    PyObject_SetAttr(mod, PyUnicode_FromString("this"), (PyObject *)obj);
}

ScriptHost::ScriptHost() {
    PyImport_AppendInittab("noon", PyInit_noon);
    Py_Initialize();
    PyImport_ImportModule("noon");
}

ScriptHost::~ScriptHost() {
    Py_Finalize();
}

bool ScriptHost::LoadFile(const string& filename) {
    ifstream file;

	file.open(filename);

    if (!file.is_open()) {
        fprintf(stderr, "Could not open file '%s'\n", filename.c_str());
        return false;
    }

    string script((istreambuf_iterator<char>(file)),
                  istreambuf_iterator<char>());

	file.close();

    PyRun_SimpleString(script.c_str());

    return true;
}

void ScriptHost::Run() {

}
