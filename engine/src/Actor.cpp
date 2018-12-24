#include "Actor.hpp"
#include "Scene.hpp"

void Actor::Update() {
    for (auto& c : _components) {
        c->Update();
    }
}

void Actor::Draw(RenderWindow * ctx) {
    for (auto& c : _components) {
        c->Draw(ctx);
    }
}

void Actor::HandleEvent(Event * evt) {
    for (auto& c : _components) {
        c->HandleEvent(evt);
    }
}

void Actor::AddComponent(unique_ptr<Component>&& comp) {
    comp->SetActor(this);
    _components.push_back(move(comp));
}

Scene * Actor::GetScene() {
    return _scene;
}

void Actor::SetScene(Scene * scene) {
    _scene = scene;
    for (auto& c : _components) {
        c->OnSceneChanged(scene);
    }
}
