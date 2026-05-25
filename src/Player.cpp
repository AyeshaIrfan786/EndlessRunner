#include "Player.h"
#include <cmath>
#include <cstdint>
#include <algorithm>

static const float GROUND_Y   = 420.f;
static const float GRAVITY    = 1900.f;
static const float JUMP_FORCE = -720.f;
static const float SLIDE_TIME = 0.55f;
static const float LANE_LERP  = 11.f;
static const float HIT_TIME   = 1.2f;

Player::Player(const CharacterData& data) {
    hearts      = data.hearts;
    maxSpeed    = data.speed;
    bodyColor   = data.bodyColor;
    accentColor = data.accentColor;
    currentLane = 1;
    laneTargetX = laneX[1];
    bounds = sf::FloatRect({laneX[1] - 20.f, GROUND_Y - 64.f}, {40.f, 64.f});
}

bool Player::isGrounded() const {
    return bounds.position.y + bounds.size.y >= GROUND_Y - 2.f;
}

void Player::jump() {
    if (isGrounded() &&
        state != PlayerState::SLIDING &&
        state != PlayerState::HIT &&
        state != PlayerState::DEAD) {
        velocityY = JUMP_FORCE;
        state     = PlayerState::JUMPING;
    }
}

void Player::slide() {
    if (isGrounded() && state == PlayerState::RUNNING) {
        state      = PlayerState::SLIDING;
        slideTimer = SLIDE_TIME;
    }
}

void Player::switchLane(int dir) {
    if (state == PlayerState::DEAD) return;
    int next = currentLane + dir;
    if (next >= 0 && next <= 2) {
        currentLane = next;
        laneTargetX = laneX[currentLane];
    }
}

void Player::takeDamage() {
    if (isInvincible() || state == PlayerState::DEAD) return;
    hearts--;
    hitTimer = HIT_TIME;
    if (hearts <= 0) state = PlayerState::DEAD;
    else             state = PlayerState::HIT;
}

void Player::update(float dt) {
    // Lane lerp
    float cx = bounds.position.x + bounds.size.x * 0.5f;
    bounds.position.x += (laneTargetX - cx) * LANE_LERP * dt;

    // Hit timer
    if (hitTimer > 0.f) {
        hitTimer -= dt;
        if (hitTimer <= 0.f && state == PlayerState::HIT)
            state = PlayerState::RUNNING;
    }

    // Slide countdown
    if (state == PlayerState::SLIDING) {
        slideTimer -= dt;
        if (slideTimer <= 0.f) state = PlayerState::RUNNING;
    }

    // Hitbox shrink for slide — only change SIZE not position
    float targetH = (state == PlayerState::SLIDING) ? 32.f : 64.f;
    bounds.size.y += (targetH - bounds.size.y) * 14.f * dt;

    // Gravity — always apply
    velocityY         += GRAVITY * dt;
    bounds.position.y += velocityY * dt;

    // Landing
    if (bounds.position.y + bounds.size.y >= GROUND_Y) {
        bounds.position.y = GROUND_Y - bounds.size.y;
        velocityY = 0.f;
        if (state == PlayerState::JUMPING)
            state = PlayerState::RUNNING;
    }

    // Animations
    if (state == PlayerState::RUNNING) bobTimer += dt * 9.f;
    animTimer += dt;
}

void Player::draw(sf::RenderWindow& window) {
    // Flicker when invincible
    if (isInvincible() && (int)(hitTimer * 10) % 2 == 0) return;

    float x   = bounds.position.x;
    float y   = bounds.position.y;
    float w   = bounds.size.x;
    float h   = bounds.size.y;
    float bob = (state == PlayerState::RUNNING)
                ? std::sin(bobTimer) * 3.f : 0.f;

    if (state == PlayerState::SLIDING) {
        sf::RectangleShape body({w + 16.f, h});
        body.setPosition({x - 8.f, y + bob});
        body.setFillColor(bodyColor);
        window.draw(body);

        sf::RectangleShape visor({w, 8.f});
        visor.setPosition({x, y + 4.f + bob});
        visor.setFillColor(sf::Color(accentColor.r, accentColor.g, accentColor.b, 200));
        window.draw(visor);
        return;
    }

    // Shadow
    sf::RectangleShape shadow({w + 8.f, 5.f});
    shadow.setPosition({x - 4.f, GROUND_Y - 3.f});
    shadow.setFillColor(sf::Color(0, 0, 0, 50));
    window.draw(shadow);

    // Legs
    float legBob1 = std::sin(bobTimer) * 7.f;
    float legBob2 = std::sin(bobTimer + 3.14159f) * 7.f;
    float legH    = h * 0.28f;

    sf::RectangleShape leg1({w * 0.35f, legH});
    leg1.setPosition({x + w * 0.08f, y + h * 0.7f + legBob1 + bob});
    leg1.setFillColor(sf::Color(
        (uint8_t)(bodyColor.r * 0.6f),
        (uint8_t)(bodyColor.g * 0.6f),
        (uint8_t)(bodyColor.b * 0.6f)));
    window.draw(leg1);

    sf::RectangleShape leg2({w * 0.35f, legH});
    leg2.setPosition({x + w * 0.57f, y + h * 0.7f + legBob2 + bob});
    leg2.setFillColor(leg1.getFillColor());
    window.draw(leg2);

    // Body
    sf::RectangleShape body({w, h * 0.68f});
    body.setPosition({x, y + bob});
    body.setFillColor(bodyColor);
    window.draw(body);

    // Chest stripe
    sf::RectangleShape stripe({w, 7.f});
    stripe.setPosition({x, y + h * 0.3f + bob});
    stripe.setFillColor(sf::Color(accentColor.r, accentColor.g, accentColor.b, 110));
    window.draw(stripe);

    // Head
    sf::RectangleShape head({w * 0.82f, h * 0.26f});
    head.setPosition({x + w * 0.09f, y - h * 0.26f + bob});
    head.setFillColor(sf::Color(
        (uint8_t)std::min(255, (int)bodyColor.r + 30),
        (uint8_t)std::min(255, (int)bodyColor.g + 30),
        (uint8_t)std::min(255, (int)bodyColor.b + 30)));
    window.draw(head);

    // Visor
    sf::RectangleShape visor({w * 0.6f, 7.f});
    visor.setPosition({x + w * 0.2f, y - h * 0.18f + bob});
    visor.setFillColor(sf::Color(accentColor.r, accentColor.g, accentColor.b, 210));
    window.draw(visor);

    // Antenna
    sf::RectangleShape ant({2.f, 10.f});
    ant.setPosition({x + w * 0.5f, y - h * 0.38f + bob});
    ant.setFillColor(accentColor);
    window.draw(ant);

    sf::RectangleShape antTip({6.f, 6.f});
    antTip.setPosition({x + w * 0.5f - 2.f, y - h * 0.44f + bob});
    antTip.setFillColor(accentColor);
    window.draw(antTip);
}