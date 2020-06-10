#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <Noon/Macros.hpp>

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::Vector2f;
using sf::Event;

#include <string>
using std::string;

class Actor;
class Scene;

class Component {
public:

    Component() = default;

    virtual ~Component() = default;

    Actor* GetActor();

    virtual void SetActor(Actor * actor);

    virtual void Update() { }

    virtual void Draw(RenderWindow * ctx) { UNUSED(ctx); }

    virtual void HandleEvent(Event * evt) { UNUSED(evt); }

    virtual void OnSceneChanged(Scene * scene) { UNUSED(scene); }

private:

    Actor * _actor;

};

#endif //COMPONENT_HPP
