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

#include "Engine.gen.hpp"

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

    mod = PyModule_Create(&NoonModule);
    if (!mod) {
        return nullptr;
    }

    _Script_InitGen(mod);

    return mod;
}

void ScriptHost::SetOwner(Actor * actor) {
    PyObject * mod = PyImport_GetModule(PyUnicode_FromString("__main__"));
    PyObject_SetAttr(mod, PyUnicode_FromString("this"), new_ActorType(actor));
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
