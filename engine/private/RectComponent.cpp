#include <Noon/RectComponent.hpp>
#include <Noon/Actor.hpp>

void RectComponent::Draw(RenderWindow * ctx)
{
    _shape.setPosition(GetActor()->GetPosition());
    _shape.setRotation(GetActor()->GetRotation());
    _shape.setScale(GetActor()->GetScale());
    ctx->draw(_shape);
}