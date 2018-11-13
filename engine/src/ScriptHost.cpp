#include "ScriptHost.hpp"

#include <Script.hpp>

#include <fstream>
using std::ifstream;
using std::istreambuf_iterator;

#include <SFML/System.hpp>
using sf::Vector2f;
using sf::Vector2i;
using sf::Vector2u;

#include <add_on/datetime/datetime.h>
#include <add_on/scriptarray/scriptarray.h>
#include <add_on/scriptstdstring/scriptstdstring.h>

void as_print(string& str) {
    printf("%s\n", str.c_str());
}

void asMessageCallback(const asSMessageInfo *msg, void *param)
{
    if (msg->type == asMSGTYPE_INFORMATION)
	{
        printf("%s (%d:%d) %s\n", msg->section, msg->row, msg->col, msg->message);
    }
	else if (msg->type == asMSGTYPE_WARNING)
    {
        fprintf(stderr, "%s (%d:%d) %s\n", msg->section, msg->row, msg->col, msg->message);
    }
    else if (msg->type == asMSGTYPE_ERROR)
    {
        fprintf(stderr, "%s (%d:%d) %s\n", msg->section, msg->row, msg->col, msg->message);
    }
}

ScriptHost::ScriptHost() {
    _as = asCreateScriptEngine();
    if (!_as) {
        fprintf(stderr, "Failed to create AngelScript Engine\n");
        return;
    }

	_as->SetMessageCallback(asFUNCTION(asMessageCallback), 0, asCALL_CDECL);

    RegisterScriptDateTime(_as);
    RegisterScriptArray(_as, true);
    RegisterStdString(_as);

    _as->RegisterObjectType("Vector2f", sizeof(Vector2f), asOBJ_VALUE | asGetTypeTraits<Vector2f>());
    _as->RegisterObjectProperty("Vector2f", "float x", asOFFSET(Vector2f,x));
    _as->RegisterObjectProperty("Vector2f", "float y", asOFFSET(Vector2f,y));
    _as->RegisterObjectBehaviour("Vector2f", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<Vector2f>), asCALL_CDECL_OBJLAST);
    _as->RegisterObjectBehaviour("Vector2f", asBEHAVE_CONSTRUCT, "void f(const Vector2f &in)", asFUNCTION(ScriptCopyCtor<Vector2f>), asCALL_CDECL_OBJLAST);
    _as->RegisterObjectBehaviour("Vector2f", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<Vector2f>), asCALL_CDECL_OBJLAST);
    _as->RegisterObjectMethod("Vector2f", "Vector2f &opAssign(const Vector2f &in)", asMETHODPR(Vector2f, operator =, (const Vector2f&), Vector2f&), asCALL_THISCALL);

    //_as->RegisterObjectType("Vector2i", sizeof(Vector2i), asOBJ_VALUE | asGetTypeTraits<Vector2i>());
    //_as->RegisterObjectBehaviour("Vector2u", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<Vector2i>), asCALL_CDECL_OBJLAST);
    //_as->RegisterObjectBehaviour("Vector2u", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<Vector2i>), asCALL_CDECL_OBJLAST);

    //_as->RegisterObjectType("Vector2u", sizeof(Vector2u), asOBJ_VALUE | asGetTypeTraits<Vector2u>());
    //_as->RegisterObjectBehaviour("Vector2u", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<Vector2u>), asCALL_CDECL_OBJLAST);
    //_as->RegisterObjectBehaviour("Vector2u", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<Vector2u>), asCALL_CDECL_OBJLAST);

    _as->RegisterObjectType("App", 0, asOBJ_REF | asOBJ_NOCOUNT);

    _as->RegisterGlobalFunction("void Print(string &in)", asFUNCTION(as_print), asCALL_CDECL);

    _RegisterGeneratedScript(_as);
}

ScriptHost::~ScriptHost() {
    for (auto asCtx : _asContexts) {
        asCtx->Release();
    }

    if (_as) {
        _as->ShutDownAndRelease();
    }
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

    asIScriptModule * mod = _as->GetModule(0, asGM_ALWAYS_CREATE);
    mod->AddScriptSection("script", script.data(), script.size());
    mod->Build();

    _asMain = _as->GetModule(0)->GetFunctionByDecl("void main()");

    return true;
}

void ScriptHost::Run() {
    if (!_asMain) {
        return;
    }

	asIScriptContext * asCtx = GetContext();

	asCtx->Prepare(_asMain);
	asCtx->Execute();

	ReturnContext(asCtx);
}

asIScriptContext * ScriptHost::GetContext() {
	if (_asContexts.empty()) {
		return _as->CreateContext();
	}

	asIScriptContext * asCtx = _asContexts.back();
	_asContexts.pop_back();
	return asCtx;
}

void ScriptHost::ReturnContext(asIScriptContext * asCtx) {
	_asContexts.push_back(asCtx);
}
