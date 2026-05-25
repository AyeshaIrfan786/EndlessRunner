#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, float w, float h, sf::Color col)
    : color(col) {
    bounds = sf::FloatRect({x, y}, {w, h});
    baseY  = y;
}

void Obstacle::update(float dt, float gameSpeed) {
    bounds.position.x -= 320.f * gameSpeed * dt;
}

void Obstacle::draw(sf::RenderWindow& window) {
    sf::RectangleShape r(bounds.size);
    r.setPosition(bounds.position);
    r.setFillColor(color);
    window.draw(r);
}

bool Obstacle::isOffScreen() const {
    return bounds.position.x + bounds.size.x < 0.f;
}