#include <Noon/Component.hpp>
#include <Noon/Actor.hpp>
#include <Noon/Scene.hpp>

Actor * Component::GetActor() { 
    return _actor; 
}

void Component::SetActor(Actor * actor) {
    _actor = actor;
    OnActorChanged(_actor);
}
