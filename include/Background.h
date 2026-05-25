#pragma once
#include <SFML/Graphics.hpp>

class Background {
public:
    Background();
    void update(float dt, float gameSpeed);
    void draw(sf::RenderWindow& window);
private:
    float offsets[3] = {0.f, 0.f, 0.f};
    const float speeds[3] = {0.12f, 0.35f, 0.75f};
};