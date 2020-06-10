#include <Noon/App.hpp>

App * App::_Inst = nullptr;

App::App() {
    _Inst = this;
    _sfWindow.reset(new RenderWindow(VideoMode(1024, 768), "Noon"));
}

void App::Start() {
    Event e;
    while (_sfWindow->isOpen()) {
        while (_sfWindow->pollEvent(e)) {
            if (e.type == Event::Closed) {
                _sfWindow->close();
            }

            if (_scene) {
                _scene->HandleEvent(&e);
            }
        }

        Update();
        Draw();
    }
}

void App::Update() {
    if (_scene) {
        _scene->Update();
    }
}

void App::Draw() {
    _sfWindow->clear(Color::Black);

    if (_scene) {
        _scene->Draw(_sfWindow.get());
    }

    _sfWindow->display();
}

void App::SetScene(unique_ptr<Scene>&& scene) {
    _scene = move(scene);
}
