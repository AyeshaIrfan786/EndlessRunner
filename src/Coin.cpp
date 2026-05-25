#include "Coin.h"
#include <cmath>
#include <cstdint>

Coin::Coin(float x, float y) {
    bounds = sf::FloatRect({x - 10.f, y}, {20.f, 20.f});
    baseY  = y;
}

void Coin::update(float dt, float gameSpeed) {
    bobTimer          += dt * 4.f;
    bounds.position.x -= 320.f * gameSpeed * dt;
    bounds.position.y  = baseY + std::sin(bobTimer) * 5.f;
}

void Coin::draw(sf::RenderWindow& window) {
    if (collected) return;

    sf::CircleShape outer(10.f);
    outer.setPosition(bounds.position);
    outer.setFillColor(sf::Color(220, 180, 0));
    window.draw(outer);

    sf::CircleShape inner(5.f);
    inner.setPosition({bounds.position.x + 5.f, bounds.position.y + 5.f});
    inner.setFillColor(sf::Color(255, 230, 80));
    window.draw(inner);
}