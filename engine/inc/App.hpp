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

#include "Script.hpp"
#include "Scene.hpp"

SCRIPT_CLASS(App)
class App
{
public:

    static App * Inst() { return _Inst; }

    App();

    virtual void Start();

    virtual void Update();

    virtual void Draw();

    virtual void SetScene(unique_ptr<Scene>&& scene);

private:

    static App * _Inst;

    unique_ptr<Scene> _scene;

    unique_ptr<RenderWindow> _sfWindow;

};

#endif // APP_HPP
