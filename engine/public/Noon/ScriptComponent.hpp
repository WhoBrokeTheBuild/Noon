#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <Noon/Component.hpp>
#include <Noon/Macros.hpp>

#include <Python.h>

#include <string>
using std::string;

#include <vector>
using std::vector;

class ScriptComponent : public Component
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptComponent)

    ScriptComponent(const std::string& filename);

	virtual ~ScriptComponent();

    virtual void OnActorChanged(Actor * actor) override;

private:

    std::string _filename;

    PyObject * _pyLocals;

};


#endif // SCRIPT_COMPONENT_HPP
