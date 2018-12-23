#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::Vector2f;
using sf::Event;

#include <string>
using std::string;

class Actor;

class Component {
public:

    Component(Actor * a) : _actor(a) { }

    Actor* GetActor() { 
        return _actor; 
    }

    void SetActor(Actor * a) {
        _actor = a;
    }

    virtual void Update();

    virtual void Draw(RenderWindow * ctx);

    virtual void HandleEvent(Event * evt);

private:

    Actor* _actor;
};

#endif //COMPONENT_HPP