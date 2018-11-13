#include <Noon.hpp>

#if defined(EDITOR_BUILD)
#include <Editor.hpp>
#endif

#include <SFML/Graphics.hpp>
using sf::RectangleShape;
using sf::Vector2f;
using sf::Color;
using sf::Event;

#include <memory>
using std::unique_ptr;

class Player : public Actor
{
public:

    Player()
        : Actor() {
        SetPosition(Vector2f(100.0f, 100.0f));
        _shape.setSize(Vector2f(50.0f, 50.0f));
        _shape.setFillColor(Color::Red);

        _script.SetOwner(this);
        _script.LoadFile("assets/test.py");

        SetRotation(45.0f);
        _script.Run();
    }

    virtual void Draw(RenderWindow * ctx) override {
        _shape.setPosition(GetPosition());
        _shape.setRotation(GetRotation());
        _shape.setScale(GetScale());
        ctx->draw(_shape);
    }

    virtual void HandleEvent(Event * evt) override {
        if (evt->type == Event::KeyPressed) {
            printf("Key Pressed %d\n", evt->key.code);
        }
    }

private:

    RectangleShape _shape;
    ScriptHost _script;

};

int main(int argc, char** argv)
{
#if defined(EDITOR_BUILD)
    Editor app;
#else
    App app;
#endif

    app.Add(unique_ptr<Actor>(new Player));

    app.Start();

    return 0;
}
