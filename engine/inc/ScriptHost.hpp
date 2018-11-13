#ifndef SCRIPT_HOST_HPP
#define SCRIPT_HOST_HPP

#include <angelscript.h>

#include <string>
using std::string;

#include <vector>
using std::vector;

class ScriptHost
{
public:

    ScriptHost();
	virtual ~ScriptHost();

    bool LoadFile(const string& filename);

    void Run();

    asIScriptEngine * GetEngine() { return _as; }

private:

	asIScriptContext * GetContext();
	void ReturnContext(asIScriptContext * asCtx);

    asIScriptEngine * _as;
    asIScriptFunction * _asMain;

	vector<asIScriptContext *> _asContexts;

};


#endif // SCRIPT_HOST_HPP
