#pragma once
#include <SFML/Graphics.hpp>
#include "CharacterSelectScreen.h"

enum class PlayerState { RUNNING, JUMPING, SLIDING, HIT, DEAD };

class Player {
public:
    sf::FloatRect bounds;
    PlayerState   state     = PlayerState::RUNNING;
    int           hearts;
    float         maxSpeed;

    static constexpr float laneX[3] = {160.f, 400.f, 640.f};
    int   currentLane  = 1;
    float laneTargetX  = 400.f;

    float velocityY    = 0.f;
    float slideTimer   = 0.f;
    float hitTimer     = 0.f;   // invincibility after hit
    float bobTimer     = 0.f;
    float animTimer    = 0.f;

    sf::Color bodyColor;
    sf::Color accentColor;

    Player() {}
    Player(const CharacterData& data);

    void jump();
    void slide();
    void switchLane(int dir);
    void takeDamage();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isGrounded() const;
    bool isInvincible() const { return hitTimer > 0.f; }
};