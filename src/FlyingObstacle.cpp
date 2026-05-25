#include "FlyingObstacle.h"

FlyingObstacle::FlyingObstacle(float laneX)
    : Obstacle(laneX - 26.f, 300.f, 52.f, 26.f, sf::Color(140, 0, 220)) {}

void FlyingObstacle::draw(sf::RenderWindow& window) {
    // Body
    sf::RectangleShape body(bounds.size);
    body.setPosition(bounds.position);
    body.setFillColor(sf::Color(110, 0, 180));
    window.draw(body);

    // Rotors
    sf::RectangleShape rL({22.f, 5.f});
    rL.setPosition({bounds.position.x - 20.f, bounds.position.y - 3.f});
    rL.setFillColor(sf::Color(180, 80, 255));
    window.draw(rL);

    sf::RectangleShape rR({22.f, 5.f});
    rR.setPosition({bounds.position.x + bounds.size.x - 2.f, bounds.position.y - 3.f});
    rR.setFillColor(sf::Color(180, 80, 255));
    window.draw(rR);

    // Eye
    sf::RectangleShape eye({10.f, 6.f});
    eye.setPosition({bounds.position.x + bounds.size.x / 2.f - 5.f,
                     bounds.position.y + 8.f});
    eye.setFillColor(sf::Color(255, 50, 50));
    window.draw(eye);

    // Exhaust
    sf::RectangleShape ex({6.f, 8.f});
    ex.setPosition({bounds.position.x + bounds.size.x / 2.f - 3.f,
                    bounds.position.y + bounds.size.y});
    ex.setFillColor(sf::Color(200, 100, 255, 140));
    window.draw(ex);
}