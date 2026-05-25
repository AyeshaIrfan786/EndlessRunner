#pragma once
#include "Obstacle.h"

class DynamicObstacle : public Obstacle {
public:
    DynamicObstacle(float laneX);
    void update(float dt, float gameSpeed) override;
    void draw(sf::RenderWindow& window) override;
private:
    float sinTimer = 0.f;
};