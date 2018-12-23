#ifndef SCENE_HPP
#define SCENE_HPP

#include "Script.hpp"
#include "Actor.hpp"

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::Event;

#include <memory>
using std::unique_ptr;

#include <vector>
using std::vector;

SCRIPT_CLASS(Scene)
class Scene 
{
public:

    virtual void Update();

    virtual void Draw(RenderWindow * ctx);

    virtual void HandleEvent(Event * evt);

    virtual void Add(unique_ptr<Actor>&& actor);

private:

    vector<unique_ptr<Actor>> _actors;

};

#endif // SCENE_HPP
