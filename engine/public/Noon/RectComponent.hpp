#ifndef RECT_COMPONENT_HPP
#define RECT_COMPONENT_HPP

#include "Component.hpp"

#include <SFML/Graphics.hpp>
using sf::RectangleShape;
using sf::Vector2f;
using sf::Color;

class RectComponent : public Component {
public:

    RectComponent(Vector2f size, Color color) {
        _shape.setSize(size);
        _shape.setFillColor(color);
    }

    virtual void Draw(RenderWindow * ctx) override;

private:

    RectangleShape _shape;
};

#endif //RECT_COMPONENT_HPP