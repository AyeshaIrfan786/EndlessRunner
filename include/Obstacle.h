#pragma once
#include "Entity.h"

class Obstacle : public Entity {
public:
    Obstacle(float x, float y, float w, float h, sf::Color col);
    void update(float dt, float gameSpeed) override;
    void draw(sf::RenderWindow& window) override;
    bool isOffScreen() const override;
protected:
    sf::Color color;
    float     baseY;
};