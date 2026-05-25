#include "DynamicObstacle.h"
#include <cmath>

DynamicObstacle::DynamicObstacle(float laneX)
    : Obstacle(laneX - 18.f, 362.f, 36.f, 52.f, sf::Color(255, 130, 0)) {}

void DynamicObstacle::update(float dt, float gameSpeed) {
    sinTimer += dt * 3.5f;
    bounds.position.x -= 320.f * gameSpeed * dt;
    bounds.position.y  = baseY + std::sin(sinTimer) * 22.f;
}

void DynamicObstacle::draw(sf::RenderWindow& window) {
    // Outer box
    sf::RectangleShape outer(bounds.size);
    outer.setPosition(bounds.position);
    outer.setFillColor(sf::Color(200, 100, 0));
    window.draw(outer);

    // Inner rotating square feel
    float s = bounds.size.x * 0.5f;
    sf::RectangleShape inner({s, s});
    inner.setPosition({
        bounds.position.x + bounds.size.x * 0.25f,
        bounds.position.y + bounds.size.y * 0.25f});
    inner.setFillColor(sf::Color(255, 180, 0));
    window.draw(inner);

    // Corner dots
    float d = 4.f;
    sf::RectangleShape dot({d, d});
    dot.setFillColor(sf::Color(255, 220, 80));
    float positions[4][2] = {
        {bounds.position.x + 4.f,                     bounds.position.y + 4.f},
        {bounds.position.x + bounds.size.x - 8.f,     bounds.position.y + 4.f},
        {bounds.position.x + 4.f,                     bounds.position.y + bounds.size.y - 8.f},
        {bounds.position.x + bounds.size.x - 8.f,     bounds.position.y + bounds.size.y - 8.f}
    };
    for (auto& p : positions) {
        dot.setPosition({p[0], p[1]});
        window.draw(dot);
    }
}