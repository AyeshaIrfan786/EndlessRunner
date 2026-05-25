#include "Particle.h"
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <algorithm>

void ParticleSystem::emit(float x, float y, sf::Color color, int count) {
    for (int i = 0; i < count; i++) {
        Particle p;
        p.pos     = {x, y};
        float ang = (rand() % 360) * 3.14159f / 180.f;
        float spd = 80.f + rand() % 200;
        p.vel     = {std::cos(ang) * spd, std::sin(ang) * spd};
        p.life    = p.maxLife = 0.35f + (rand() % 40) / 100.f;
        p.color   = color;
        particles.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
    for (auto& p : particles) {
        p.life -= dt;
        p.pos  += p.vel * dt;
        p.vel  *= 0.90f;
    }
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p){ return p.life <= 0.f; }),
        particles.end());
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    for (auto& p : particles) {
        float t = p.life / p.maxLife;
        sf::CircleShape c(2.f * t + 1.f);
        c.setPosition(p.pos);
        sf::Color col = p.color;
        col.a = (uint8_t)(200 * t);
        c.setFillColor(col);
        window.draw(c);
    }
}