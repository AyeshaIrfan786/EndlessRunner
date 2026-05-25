#pragma once
#include "Entity.h"

class Coin : public Entity {
public:
    bool  collected = false;
    float bobTimer  = 0.f;
    float baseY;

    Coin(float x, float y);
    void update(float dt, float gameSpeed) override;
    void draw(sf::RenderWindow& window) override;
};