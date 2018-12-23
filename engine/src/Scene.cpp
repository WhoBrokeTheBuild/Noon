#include "Scene.hpp"

void Scene::Update() {
    for (auto& a : _actors) {
        a->Update();
    }
}

void Scene::Draw(RenderWindow * ctx) {
    for (auto& a : _actors) {
        a->Draw(ctx);
    }
}

void Scene::HandleEvent(Event * evt) {
    for (auto& a : _actors) {
        a->HandleEvent(evt);
    }
}

void Scene::Add(unique_ptr<Actor>&& actor) {
    _actors.push_back(move(actor));
}
