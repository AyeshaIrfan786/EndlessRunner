#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

class SplashScreen {
public:
    SplashScreen(sf::RenderWindow& window, sf::Font& font);
    void handleInput(sf::Keyboard::Key key, GameState& state);
    void update(float dt, GameState& state);
    void draw();

private:
    sf::RenderWindow& window;
    sf::Font& font;

    // Phases: 0=fadein, 1=visible, 2=blinking, 3=fadeout
    int   phase       = 0;
    float phaseTimer  = 0.f;
    float fadeAlpha   = 0.f;   // 0-255
    bool  anyKeyPressed = false;

    // Background stars
    struct Star {
        float x, y, size, speed, brightness;
    };
    std::vector<Star> stars;

    // Scrolling ground lines
    float groundOffset = 0.f;

    // Blink timer for "press any key"
    float blinkTimer  = 0.f;
    bool  blinkOn     = true;

    void spawnStars();
    void drawStars();
    void drawGround();
    void drawTitle();
    void drawPressAnyKey();
    void drawFadeOverlay();
};