#include "ScriptHost.hpp"

#include <fstream>
using std::ifstream;
using std::istreambuf_iterator;

#include <SFML/System.hpp>
using sf::Vector2f;
using sf::Vector2i;
using sf::Vector2u;

ScriptHost::ScriptHost() {

}

ScriptHost::~ScriptHost() {

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



    return true;
}

void ScriptHost::Run() {

}
