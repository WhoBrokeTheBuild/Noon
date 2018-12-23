#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "Script.hpp"
#include "Component.hpp"

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::Vector2f;
using sf::Event;

#include <vector>
using std::vector;

#include <memory>
using std::unique_ptr;

class Scene;

SCRIPT_CLASS(Actor)
class Actor
{
public:

    Actor() = default;

    virtual ~Actor() = default;

    SCRIPT_METHOD(Actor, Update, "void update()")
    virtual void Update();

    virtual void Draw(RenderWindow * ctx);

    virtual void HandleEvent(Event * evt);

    virtual void AddComponent(unique_ptr<Component>&& comp);

    Scene * GetScene();

    void SetScene(Scene * scene);

    SCRIPT_METHOD(Actor, GetPosition, "vec2f get_position()")
    virtual Vector2f GetPosition() {
        return _position;
    }

    SCRIPT_METHOD(Actor, SetPosition, "void set_position(vec2f)")
    virtual void SetPosition(const Vector2f& value) {
        _position = value;
    }

    SCRIPT_METHOD(Actor, GetScale, "vec2f get_scale()")
    virtual Vector2f GetScale() {
        return _scale;
    }

    SCRIPT_METHOD(Actor, SetScale, "void set_scale(vec2f)")
    virtual void SetScale(const Vector2f& value) {
        _scale = value;
    }

    SCRIPT_METHOD(Actor, GetRotation, "float get_rotation()")
    virtual float GetRotation() {
        return _rotation;
    }

    SCRIPT_METHOD(Actor, SetRotation, "void set_rotation(float)")
    virtual void SetRotation(const float& value) {
        _rotation = value;
    }

private:

    Scene * _scene = nullptr;

    Vector2f _position = Vector2f(0.0f, 0.0f);
    Vector2f _scale    = Vector2f(1.0f, 1.0f);

    float _rotation = 0.0f;

    vector<unique_ptr<Component>> _components;

};

#endif //ACTOR_HPP
