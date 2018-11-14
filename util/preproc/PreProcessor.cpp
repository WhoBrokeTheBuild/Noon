#include <cstdio>

#include <string>
using std::string;
using std::to_string;
using std::istreambuf_iterator;

#include <algorithm>
using std::find_if;

#include <vector>
using std::vector;

#include <unordered_map>
using std::unordered_map;

#include <iostream>
using std::ostream;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <regex>
using std::regex;
using std::smatch;
using std::sregex_iterator;

#include <Util.hpp>

const regex ScriptFunctionRegex("(.*)SCRIPT_FUNCTION\\((.+?),(.*)\\)");
const regex ScriptClassRegex("(.*)SCRIPT_CLASS\\((.*)\\)");
const regex ScriptMethodRegex("(.*)SCRIPT_METHOD\\((.+?),(.+?),(.*)\\)");
const regex DefinitionRegex("(\\w+)\\s(\\w+)\\((.*)\\)");

const char * ClassDefTemplate = R"(
struct {0}Type {
    PyObject_HEAD
    {0} * _ptr;
};
)";

const char * ClassDataTemplate = R"(
static PyMethodDef {0}_methods[] = {
{1}
    {nullptr, nullptr, 0, nullptr}
};

static PyTypeObject {0}_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "noon.{0}",                                /* tp_name */
    sizeof({0}Type),                        /* tp_basicsize */
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
    {0}_methods,                         /* tp_methods */
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
)";

const char * ClassImplTemplate = R"(
    {0}_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&{0}_type) == 0) {
        Py_INCREF(&{0}_type);
        PyModule_AddObject(mod, "{0}", (PyObject *)&{0}_type);
    }
)";

const char * MethodDefTemplate = R"(    {"{1}", func_{0}_{1}, {2}, nullptr},
)";

const char * MethodImplTemplate = R"(
static PyObject * func_{0}_{1}(PyObject * self, PyObject * args) {
    {0} * type = (({0}Type *)self)->_ptr;
{2}
}
)";

const char * ParseArgTemplate = R"(    {0} {1};
    if (!PyArg_ParseTuple(args, "{3}", {2})) {
        return nullptr;
    }
)";

struct Class {
    string Name;
};

struct Function {
    string Name;
    string ReturnType;
    string FuncName;
    vector<string> Args;
};

struct Method {
    string Name;
    string ReturnType;
    string FuncName;
    vector<string> Args;
};

string GetWrapperMethod(const Class& c, const Method& m) {
    string code = "";

    const vector<string>& args = m.Args;
    const string& rt = m.ReturnType;

    string argVars = "";
    for (size_t i = 0; i < args.size(); ++i) {
        string aName = "arg" + to_string(i);

        if (args[i] == "int") {
            code += indexed_replace(ParseArgTemplate, "int", aName.c_str(), ("&" + aName).c_str(), "i");
            argVars += aName + ",";
        }
        else if (args[i] == "long") {
            code += indexed_replace(ParseArgTemplate, "long", aName.c_str(), ("&" + aName).c_str(), "l");
            argVars += aName + ",";
        }
        else if (args[i] == "float") {
            code += indexed_replace(ParseArgTemplate, "float", aName.c_str(), ("&" + aName).c_str(), "f");
            argVars += aName + ",";
        }
        else if (args[i] == "double") {
            code += indexed_replace(ParseArgTemplate, "double", aName.c_str(), ("&" + aName).c_str(), "d");
            argVars += aName + ",";
        }
        else if (args[i] == "bool") {
            code += indexed_replace(ParseArgTemplate, "unsigned char", aName.c_str(), ("&" + aName).c_str(), "b");
            argVars += "(" + aName + " == 0),";
        }
        else if (args[i] == "string") {
            code += indexed_replace(ParseArgTemplate, "const char *", aName.c_str(), ("&" + aName).c_str(), "s");
            argVars += "string(" + aName + "),";
        }
        else if (args[i] == "vec2f") {
            code += indexed_replace(ParseArgTemplate, "Vector2f", aName.c_str(), ("&" + aName + ".x, &" + aName + ".y").c_str(), "ff");
            argVars += aName + ",";
        }
        else if (args[i] == "vec2i") {
            code += indexed_replace(ParseArgTemplate, "Vector2i", aName.c_str(), ("&" + aName + ".x, &" + aName + ".y").c_str(), "ii");
            argVars += aName + ",";
        }
    }

    if (!argVars.empty()) {
        argVars.pop_back();
    }

    if (rt == "void") {
        code += "    type->" + m.Name + "(" + argVars + ");\n"
                "    Py_RETURN_NONE;";
    } else {
        code += "    auto value = type->" + m.Name + "(" + argVars + ");\n";
        if (rt == "int" || rt == "long") {
            code += "    return PyLong_FromLong(value);";
        }
        else if (rt == "float" || rt == "double") {
            code += "    return PyFloat_FromDouble(value);";
        }
        else if (rt == "bool") {
            code += "    return (value ? Py_True : Py_False);";
        }
        else if (rt == "string") {
            code += "    return PyUnicode_FromString(value.c_str());";
        }
        else if (rt == "vec2f") {
            code += "    PyObject * ret = PyTuple_New(2);\n"
                    "    PyTuple_SET_ITEM(ret, 0, PyFloat_FromDouble(value.x));\n"
                    "    PyTuple_SET_ITEM(ret, 1, PyFloat_FromDouble(value.y));\n"
                    "    return ret;";
        }
        else if (rt == "vec2i") {
            code += "    PyObject * ret = PyTuple_New(2);\n"
                    "    PyTuple_SET_ITEM(ret, 0, PyLong_FromLong(value.x));\n"
                    "    PyTuple_SET_ITEM(ret, 1, PyLong_FromLong(value.y));\n"
                    "    return ret;";
        }
    }

    return indexed_replace(MethodImplTemplate, c.Name.c_str(), m.FuncName.c_str(), code.c_str());
}

void usage(const char * name) {
    printf("Usage %s OUTPUT_FILE SOURCE_FILE...\n", name);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    string outFilename = argv[1];

    vector<string> includes;
    vector<Class> classes;
    vector<Function> functions;
    unordered_map<string, vector<Method>> methods;

    includes.push_back("Python.h");

    for (int i = 2; i < argc; ++i) {
        string filename(argv[i]);

        ifstream inFile(filename);
        string code((istreambuf_iterator<char>(inFile)),
                    istreambuf_iterator<char>());

        if (filename.find(".h") != string::npos) {
            auto last = filename.rfind('/');
            if (last == string::npos) {
                includes.push_back(filename);
            } else {
                includes.push_back(filename.substr(last + 1));
            }
        }

        auto classIt = sregex_iterator(code.begin(), code.end(), ScriptClassRegex);
        for (; classIt != sregex_iterator(); ++classIt) {
            smatch match = *classIt;
            string prefix = match[1].str();
            string name = match[2].str();

            ltrim(prefix);
            trim(name);

            if (!prefix.empty()) {
                if (prefix[0] == '#') {
                    continue;
                } else if (prefix[0] == '/' && prefix[1] == '/') {
                    continue;
                }
            }

            classes.push_back(Class{
                name,
            });
        }

        auto funcIt = sregex_iterator(code.begin(), code.end(), ScriptFunctionRegex);
        for (; funcIt != sregex_iterator(); ++funcIt) {
            smatch match = *funcIt;
            string prefix = match[1].str();
            string name = match[2].str();
            string def = match[3].str();

            ltrim(prefix);
            trim(name);
            trim(def);

            if (!prefix.empty()) {
                if (prefix[0] == '#') {
                    continue;
                } else if (prefix[0] == '/' && prefix[1] == '/') {
                    continue;
                }
            }

            string returnType;
            string funcName;
            vector<string> args;

            functions.push_back(Function{
                name,
                returnType,
                funcName,
                args,
            });
        }

        auto methIt = sregex_iterator(code.begin(), code.end(), ScriptMethodRegex);
        for (; methIt != sregex_iterator(); ++methIt) {
            smatch match = *methIt;
            string prefix = match[1].str();
            string c = match[2].str();
            string name = match[3].str();
            string def = match[4].str();

            ltrim(prefix);
            trim(c);
            trim(name);

            if (!prefix.empty()) {
                if (prefix[0] == '#') {
                    continue;
                } else if (prefix[0] == '/' && prefix[1] == '/') {
                    continue;
                }
            }

            string returnType;
            string funcName;
            vector<string> args;

            auto defIt = sregex_iterator(def.begin(), def.end(), DefinitionRegex);
            for (; defIt != sregex_iterator(); ++defIt) {
                smatch match = *defIt;
                returnType = match[1].str();
                funcName = match[2].str();
                string argsList = match[3].str();

                if (!argsList.empty()) {
                    args.push_back(string());
                    for (auto& ch : argsList) {
                        if (isspace(ch)) {
                            continue;
                        }

                        if (ch == ',') {
                            args.push_back(string());
                            continue;
                        }

                        args.back().push_back(ch);
                    }
                }
            }

            if (methods.find(c) == methods.end()) {
                methods[c] = vector<Method>();
            }

            methods[c].push_back(Method{
                name,
                returnType,
                funcName,
                args,
            });
        }
    }

    ofstream outFile(outFilename, ofstream::trunc);

    for (int i = 2; i < argc; ++i) {
        outFile << "// Processed <" << argv[i] << ">\n";
    }

    outFile << "\n";

    for (auto& inc : includes) {
        outFile << "#include <" << inc << ">\n";
    }

    for (auto& c : classes) {
        outFile << indexed_replace(ClassDefTemplate, c.Name.c_str());

        if (methods.find(c.Name) != methods.end()) {
            for (auto& m : methods[c.Name]) {
                outFile << GetWrapperMethod(c, m);
            }
        }
    }

    for (auto& c : classes) {
        string tmp = "";

        if (methods.find(c.Name) != methods.end()) {
            for (auto& m : methods[c.Name]) {
                tmp += indexed_replace(MethodDefTemplate,
                    c.Name.c_str(), m.FuncName.c_str(),
                    (m.Args.empty() ? "METH_NOARGS" : "METH_VARARGS"));
            }
        }

        outFile << indexed_replace(ClassDataTemplate, c.Name.c_str(), tmp.c_str());
    }

    outFile << "\n";
    outFile << "void _Script_InitGen(PyObject * mod) {\n";

    for (auto& c : classes) {
        outFile << indexed_replace(ClassImplTemplate, c.Name.c_str());
    }

    outFile << "}\n";

    outFile.flush();

    return 0;
}
