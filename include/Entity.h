#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    sf::FloatRect bounds;
    bool          active = true;

    virtual void update(float dt, float gameSpeed) = 0;
    virtual void draw(sf::RenderWindow& window)    = 0;
    virtual bool isOffScreen() const {
        return bounds.position.x + bounds.size.x < 0.f;
    }
    virtual ~Entity() {}
};