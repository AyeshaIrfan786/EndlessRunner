#pragma once
#include "Obstacle.h"

class FlyingObstacle : public Obstacle {
public:
    FlyingObstacle(float laneX);
    void draw(sf::RenderWindow& window) override;
};