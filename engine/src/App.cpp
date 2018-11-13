#include "App.hpp"

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

            for (auto& a : _actors) {
                a->HandleEvent(&e);
            }
        }

        Update();
        Draw();
    }
}

void App::Update() {
    for (auto& a : _actors) {
        a->Update();
    }
}

void App::Draw() {
    _sfWindow->clear(Color::Black);

    for (auto& a : _actors) {
        a->Draw(_sfWindow.get());
    }

    _sfWindow->display();
}

void App::Add(unique_ptr<Actor>&& actor) {
    _actors.push_back(move(actor));
}
