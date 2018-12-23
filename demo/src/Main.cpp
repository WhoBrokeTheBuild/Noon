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
using std::make_unique;

class Player : public Actor
{
public:

    Player()
        : Actor() {
        AddComponent(unique_ptr<Component>(new RectComponent({ 50.0f, 50.0f }, Color::Red)));
        SetPosition({ 200.0f, 200.0f });
        SetRotation(45.0f);
        SetScale({ 2.0f, 2.0f });
    }

    virtual void HandleEvent(Event * evt) override {
        if (evt->type == Event::KeyPressed) {
            printf("Key Pressed %d\n", evt->key.code);
        }
    }

private:

};

int main(int argc, char** argv)
{
#if defined(EDITOR_BUILD)
    Editor app;
#else
    App app;
#endif

    auto s = make_unique<Scene>();

    s->AddActor(unique_ptr<Actor>(new Player));

    app.SetScene(move(s));

    app.Start();

    return 0;
}
