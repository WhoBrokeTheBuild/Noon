#include <Noon/Noon.hpp>

#if defined(EDITOR_BUILD)
#include <Noon/Editor.hpp>
#endif

#include <SFML/Graphics.hpp>
using sf::RectangleShape;
using sf::Vector2f;
using sf::Vertex;
using sf::Color;
using sf::Event;

#include <memory>
using std::unique_ptr;
using std::make_unique;

#include <vector>
using std::vector;

#include <cstdlib>
#include <ctime>

class MeshComponent : public RectComponent 
{
public:

    MeshComponent() 
        : RectComponent(Vector2f(10.0f, 10.0f), sf::Color::Red) 
    { }

    virtual void OnSceneChanged(Scene * scene) override;

};

class MeshSystem : public System {
public:

    virtual void OnComponentAdded() override {
        _meshComponents = GetComponentsByType<MeshComponent>();
    }

    virtual void Draw(RenderWindow * ctx) override {
        vector<Vertex> lines;
        for (size_t i = 0; i < _meshComponents.size(); ++i) {
            for (size_t j = 0; j < _meshComponents.size(); ++j) {
                lines.push_back(Vertex(_meshComponents[i]->GetActor()->GetPosition()));
                lines.push_back(Vertex(_meshComponents[j]->GetActor()->GetPosition()));
            }
        }
        ctx->draw(lines.data(), lines.size(), sf::Lines);
    }

private:

    vector<MeshComponent *> _meshComponents;

};

void MeshComponent::OnSceneChanged(Scene * scene) {
    auto sys = scene->GetSystemByType<MeshSystem>();
    sys->AddComponent<MeshComponent>(this);
}

int main(int argc, char** argv)
{
    UNUSED(argc);
    UNUSED(argv);
    
    srand((unsigned int)time(0));

#if defined(EDITOR_BUILD)
    Editor app;
#else
    App app;
#endif

    auto s = make_unique<Scene>();

    s->AddSystem<MeshSystem>(make_unique<MeshSystem>());
    
    for (int i = 0; i < 10; i++) {
        auto a = make_unique<Actor>();
        a->SetPosition(Vector2f(
            float(rand() % 1024),
            float(rand() % 768)
        ));
        auto mc = make_unique<MeshComponent>();
        a->AddComponent(move(mc));
        s->AddActor(move(a));
    }

    app.SetScene(move(s));

    app.Start();

    return 0;
}
