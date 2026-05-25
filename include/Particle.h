#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Particle {
    sf::Vector2f pos, vel;
    float        life, maxLife;
    sf::Color    color;
};

class ParticleSystem {
public:
    void emit(float x, float y, sf::Color color, int count = 12);
    void update(float dt);
    void draw(sf::RenderWindow& window);
private:
    std::vector<Particle> particles;
};