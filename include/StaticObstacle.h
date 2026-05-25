#pragma once
#include "Obstacle.h"

class StaticObstacle : public Obstacle {
public:
    StaticObstacle(float laneX);
    void draw(sf::RenderWindow& window) override;
};
