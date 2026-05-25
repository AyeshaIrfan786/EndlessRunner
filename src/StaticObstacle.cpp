#include "StaticObstacle.h"

StaticObstacle::StaticObstacle(float laneX)
    : Obstacle(laneX - 20.f, 356.f, 40.f, 64.f, sf::Color(200, 40, 40)) {}

void StaticObstacle::draw(sf::RenderWindow& window) {
    // Base
    sf::RectangleShape base({bounds.size.x, bounds.size.y * 0.55f});
    base.setPosition({bounds.position.x, bounds.position.y + bounds.size.y * 0.45f});
    base.setFillColor(sf::Color(160, 30, 30));
    window.draw(base);

    // Spike
    sf::ConvexShape spike;
    spike.setPointCount(3);
    float cx = bounds.position.x + bounds.size.x * 0.5f;
    spike.setPoint(0, {cx, bounds.position.y});
    spike.setPoint(1, {bounds.position.x, bounds.position.y + bounds.size.y * 0.48f});
    spike.setPoint(2, {bounds.position.x + bounds.size.x,
                       bounds.position.y + bounds.size.y * 0.48f});
    spike.setFillColor(sf::Color(230, 60, 60));
    window.draw(spike);

    // Glow top
    sf::RectangleShape tip({6.f, 6.f});
    tip.setPosition({cx - 3.f, bounds.position.y - 2.f});
    tip.setFillColor(sf::Color(255, 100, 100, 180));
    window.draw(tip);
}