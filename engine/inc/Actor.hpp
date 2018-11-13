#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <Script.hpp>

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::Vector2f;
using sf::Event;

#include <string>
using std::string;

SCRIPT_CLASS(Actor)
class Actor : public IScriptClass
{
public:

    Actor();

    virtual void Update() { }
    virtual void Draw(RenderWindow * ctx) { }
    virtual void HandleEvent(Event * evt) { }

    string GetName() { return "Actor"; }

    //SCRIPT_MEMBER(Actor, GetPosition, "Vector2f GetPosition()")
    virtual Vector2f GetPosition() {
        return _position;
    }

    SCRIPT_MEMBER(Actor, SetPosition, "void SetPosition(Vector2f &in)")
    virtual void SetPosition(const Vector2f& value) {
        _position = value;
    }

    //SCRIPT_MEMBER(Actor, GetScale, "Vector2f GetScale()")
    virtual Vector2f GetScale() {
        return _scale;
    }

    SCRIPT_MEMBER(Actor, SetScale, "void SetScale(Vector2f &in)")
    virtual void SetScale(const Vector2f& value) {
        _scale = value;
    }

    SCRIPT_MEMBER(Actor, GetRotation, "float GetRotation()")
    virtual float GetRotation() {
        return _rotation;
    }

    SCRIPT_MEMBER(Actor, SetRotation, "void SetRotation(float &in)")
    virtual void SetRotation(const float& value) {
        _rotation = value;
    }

private:

    Vector2f _position = Vector2f(0.0f, 0.0f);
    Vector2f _scale    = Vector2f(1.0f, 1.0f);

    float _rotation = 0.0f;

};

#endif //ACTOR_HPP
