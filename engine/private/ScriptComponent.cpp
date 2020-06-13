#include <Noon/ScriptComponent.hpp>
#include <Noon/App.hpp>
#include <Noon/Actor.hpp>
#include <Noon/Script.hpp>

#include <fstream>
using std::ifstream;
using std::istreambuf_iterator;

extern PyObject * new_ActorType(Actor * ptr);

ScriptComponent::ScriptComponent(const std::string& filename)
    : _filename(filename)
{
    _pyLocals = PyDict_New();
    Py_INCREF(_pyLocals);
}

ScriptComponent::~ScriptComponent()
{
    Py_DECREF(_pyLocals);
}

void ScriptComponent::OnActorChanged(Actor * actor)
{
    PyDict_SetItemString(_pyLocals, "this", new_ActorType(actor));

    ifstream file;

    printf("Loading %s\n", _filename.c_str());

	file.open(_filename);

    if (!file.is_open()) {
        fprintf(stderr, "Could not open file '%s'\n", _filename.c_str());
        return;
    }

    string script((istreambuf_iterator<char>(file)),
                  istreambuf_iterator<char>());

	file.close();

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);

    PyRun_String(script.c_str(), Py_file_input, pyMainDict, _pyLocals);
}