#include <Noon/Editor.hpp>

Editor * Editor::_Inst = nullptr;

Editor::Editor()
    : App() {
    _Inst = this;
}

void Editor::Start() {
    App::Start();
}
