#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Event;
using sf::Color;
using sf::Vector2u;

#include <memory>
using std::make_unique;
using std::unique_ptr;
using std::move;

#include <vector>
using std::vector;

#include "Actor.hpp"
#include "ScriptHost.hpp"

SCRIPT_CLASS(App)
class App
{
public:

    static App * Inst() { return _Inst; }

    App();

    virtual void Start();

    virtual void Update();
    virtual void Draw();

    virtual void Add(unique_ptr<Actor>&& actor);

private:

    static App * _Inst;

    unique_ptr<RenderWindow> _sfWindow;

    vector<unique_ptr<Actor>> _actors;

};

#endif // APP_HPP
