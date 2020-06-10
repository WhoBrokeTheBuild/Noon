#include <Noon/Scene.hpp>

void Scene::Update() {
    for (auto& a : _actors) {
        a->Update();
    }
    for (auto& sys : _systems) {
        sys.second->Update();
    }
}

void Scene::Draw(RenderWindow * ctx) {
    for (auto& a : _actors) {
        a->Draw(ctx);
    }
    for (auto& sys : _systems) {
        sys.second->Draw(ctx);
    }
}

void Scene::HandleEvent(Event * evt) {
    for (auto& a : _actors) {
        a->HandleEvent(evt);
    }
    for (auto& sys : _systems) {
        sys.second->HandleEvent(evt);
    }
}

void Scene::AddActor(unique_ptr<Actor>&& actor) {
    actor->SetScene(this);
    _actors.push_back(move(actor));
}
