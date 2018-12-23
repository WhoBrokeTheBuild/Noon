#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Component.hpp"

#include <typeinfo>
using std::type_info;

#include <typeindex>
using std::type_index;

#include <vector>
using std::vector;

#include <unordered_map>
using std::unordered_map;

class System {
public:

    virtual void Update() { }

    virtual void Draw(RenderWindow * ctx) { }

    virtual void HandleEvent(Event * evt) { }

    template <typename T>
    void AddComponent(Component * comp);

    vector<Component *> GetComponents() { 
        return _components; 
    }

    template <typename T>
    vector<T*> GetComponentsByType();

private:

    vector<Component *> _components;

    unordered_map<type_index, vector<Component *>> _componentsByType;

};

template <typename T>
void System::AddComponent(Component * comp) {
    auto id = type_index(typeid(T));
    _components.push_back(comp);
    if (_componentsByType.find(id) != _componentsByType.end()) {
        _componentsByType[id] = vector<Component *>();
    }
    _componentsByType[id].push_back(comp);
}

template <typename T>
vector<T*> System::GetComponentsByType() {
    auto id = type_index(typeid(T));
    auto list = vector<T*>();
    if (_componentsByType.find(id) != _componentsByType.end()) {
        for (auto c : _componentsByType[id]) {
            list.push_back(dynamic_cast<T*>(c));
        }
    }
    return list;
}

#endif // SYSTEM_HPP
