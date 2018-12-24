#include "Component.hpp"
#include "Actor.hpp"
#include "Scene.hpp"

Actor * Component::GetActor() { 
    return _actor; 
}

void Component::SetActor(Actor * actor) {
    _actor = actor;
}
