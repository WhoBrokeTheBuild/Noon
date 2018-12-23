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
        _shape.setSize({ 50.0f, 50.0f });
        _shape.setFillColor(Color::Red);
        SetPosition({ 200.0f, 200.0f });
        SetRotation(45.0f);
        SetScale({ 2.0f, 2.0f });
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

};

int main(int argc, char** argv)
{
#if defined(EDITOR_BUILD)
    Editor app;
#else
    App app;
#endif

    auto s = make_unique<Scene>();
    s->Add(unique_ptr<Actor>(new Player));
    app.SetScene(move(s));

    app.Start();

    return 0;
}
