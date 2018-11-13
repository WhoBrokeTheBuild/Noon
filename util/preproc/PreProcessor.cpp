#include <cstdio>

#include <string>
using std::string;
using std::istreambuf_iterator;

#include <algorithm>
using std::find_if;

#include <vector>
using std::vector;

#include <iostream>
using std::ostream;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <regex>
using std::regex;
using std::smatch;
using std::sregex_iterator;

const regex ScriptTypeRegex("(.*)SCRIPT_TYPE\\((.*)\\)");
const regex ScriptFunctionRegex("(.*)SCRIPT_FUNCTION\\((.*),(.*)\\)");
const regex ScriptClassRegex("(.*)SCRIPT_CLASS\\((.*)\\)");
const regex ScriptMethodRegex("(.*)SCRIPT_MEMBER\\((.*),(.*),(.*)\\)");

struct Type {
    string Name;
};

ostream & operator << (ostream &out, const Type& value) {
    out << "";
    return out;
}

struct Class {
    string Name;
};

ostream & operator << (ostream &out, const Class& value) {
    out << "";
    return out;
}

struct Function {
    string Name;
    string Definition;
};

ostream & operator << (ostream &out, const Function& value) {
    out << "";
    return out;
}

struct Method {
    string Name;
    string Class;
    string Definition;
};

ostream & operator << (ostream &out, const Method& method) {
    out << "";
    return out;
}

static inline void ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
        return !isspace(ch);
    }));
}

static inline void rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(string &s) {
    ltrim(s);
    rtrim(s);
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
    vector<Type> types;
    vector<Class> classes;
    vector<Function> functions;
    vector<Method> methods;

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

        for (auto it = sregex_iterator(code.begin(), code.end(), ScriptTypeRegex);
            it != sregex_iterator();
            ++it) {
            smatch match = *it;
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

            types.push_back(Type{
                .Name = name,
            });
        }

        for (auto it = sregex_iterator(code.begin(), code.end(), ScriptClassRegex);
            it != sregex_iterator();
            ++it) {
            smatch match = *it;
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
                .Name = name,
            });
        }

        for (auto it = sregex_iterator(code.begin(), code.end(), ScriptFunctionRegex);
            it != sregex_iterator();
            ++it) {
            smatch match = *it;
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

            functions.push_back(Function{
                .Name = name,
                .Definition = def,
            });
        }

        for (auto it = sregex_iterator(code.begin(), code.end(), ScriptMethodRegex);
            it != sregex_iterator();
            ++it) {
            smatch match = *it;
            string prefix = match[1].str();
            string c = match[2].str();
            string name = match[3].str();
            string def = match[4].str();

            ltrim(prefix);
            trim(c);
            trim(name);
            trim(def);

            if (!prefix.empty()) {
                if (prefix[0] == '#') {
                    continue;
                } else if (prefix[0] == '/' && prefix[1] == '/') {
                    continue;
                }
            }

            methods.push_back(Method{
                .Class = c,
                .Name = name,
                .Definition = def,
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

    outFile << "\n";
    outFile << "void _RegisterGeneratedScript() {\n";

    for (auto& type : types) {
        outFile << type;
    }

    for (auto& c : classes) {
        outFile << c;
    }

    for (auto& func : functions) {
        outFile << func;
    }

    for (auto& member : methods) {
        outFile << member;
    }

    outFile << "}\n";

    outFile.flush();

    return 0;
}
