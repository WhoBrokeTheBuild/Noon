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

        asIScriptEngine * as = _script.GetEngine();

        as->RegisterObjectType("Player", 0, asOBJ_REF);
        as->RegisterObjectBehaviour("Player", asBEHAVE_ADDREF, "void f()", asMETHOD(Player,Script_AddRef), asCALL_THISCALL);
        as->RegisterObjectBehaviour("Player", asBEHAVE_RELEASE, "void f()", asMETHOD(Player,Script_Release), asCALL_THISCALL);

        as->RegisterObjectMethod("Actor", "Player@ opCast()", asFUNCTION((ScriptCast<Actor,Player>)), asCALL_CDECL_OBJLAST);
        as->RegisterObjectMethod("Player", "Actor@ opImplCast()", asFUNCTION((ScriptCast<Player,Actor>)), asCALL_CDECL_OBJLAST);
        as->RegisterObjectMethod("Actor", "const Player@ opCast() const", asFUNCTION((ScriptCast<Actor,Player>)), asCALL_CDECL_OBJLAST);
        as->RegisterObjectMethod("Player", "const Actor@ opImplCast() const", asFUNCTION((ScriptCast<Player,Actor>)), asCALL_CDECL_OBJLAST);

        as->RegisterGlobalProperty("Player this", this);

        _script.LoadFile("assets/test.as");

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
