#include "Component.hpp"
#include "Actor.hpp"

Actor * Component::GetActor() { 
    return _actor; 
}

void Component::SetActor(Actor * actor) {
    _actor = actor;
}
