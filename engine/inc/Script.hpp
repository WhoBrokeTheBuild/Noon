#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <angelscript.h>

#define SCRIPT_TYPE(Type)
#define SCRIPT_FUNCTION(Func, Def)

#define SCRIPT_CLASS(Class)
#define SCRIPT_MEMBER(Class, Func, Def)

extern void _RegisterGeneratedScript(asIScriptEngine * as);

class IScriptClass {
public:

    void Script_AddRef() {
        ++_ref;
    }

    void Script_Release() {
        --_ref;
        if (_ref == 0) {
            delete this;
        }
    }

private:

    unsigned int _ref = 1;

};

template <typename T>
void ScriptCtor(void * ptr) {
    new(ptr) T();
}

template <typename T>
void ScriptCopyCtor(const T& other, T * ptr) {
    new(ptr) T(other);
}

template <typename T>
void ScriptDtor(void * ptr) {
    ((T*)ptr)->~T();
}

template<class A, class B>
B * ScriptCast(A * a)
{
    if (!a) return 0;
    B * b = dynamic_cast<B*>(a);
    if (b != 0) {
        b->Script_AddRef();
    }
    return b;
}

#endif // SCRIPT_HPP
