#ifndef SCENE_HPP
#define SCENE_HPP

#include "Script.hpp"
#include "Actor.hpp"
#include "System.hpp"

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::Event;

#include <typeinfo>
using std::type_info;

#include <typeindex>
using std::type_index;

#include <memory>
using std::unique_ptr;

#include <vector>
using std::vector;

#include <unordered_map>
using std::vector;

SCRIPT_CLASS(Scene)
class Scene 
{
public:

    virtual ~Scene() = default;

    virtual void Update();

    virtual void Draw(RenderWindow * ctx);

    virtual void HandleEvent(Event * evt);

    virtual void AddActor(unique_ptr<Actor>&& actor);

    template <typename T>
    void AddSystem(unique_ptr<System>&& sys);

    template <typename T>
    T * GetSystemByType();

private:

    vector<unique_ptr<Actor>> _actors;

    unordered_map<type_index, unique_ptr<System>> _systems;

};

template <typename T>
void Scene::AddSystem(unique_ptr<System>&& sys) {
    auto id = type_index(typeid(T));
    _systems[id] = move(sys);
}

template <typename T>
T * Scene::GetSystemByType() {
    auto id = type_index(typeid(T));
    System * sys = _systems[id].get();
    return dynamic_cast<T *>(sys);
}

#endif // SCENE_HPP
