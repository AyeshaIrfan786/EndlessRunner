#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include <vector>
#include <string>

class MenuScreen {
public:
    MenuScreen(sf::RenderWindow& window, sf::Font& font);
    void handleInput(sf::Keyboard::Key key, GameState& state);
    void update(float dt);
    void draw();

private:
    sf::RenderWindow& window;
    sf::Font&         font;

    // Menu items
    int selectedItem = 0;
    std::vector<std::string> items = {
        "ENDLESS MODE",
        "SURVIVAL MODE",
        "EXIT"
    };

    // Animations
    float pulseTimer  = 0.f;
    float scanline    = 0.f;
    float titleBob    = 0.f;
    float fadeAlpha   = 0.f;   // fade in on enter
    bool  fadingIn    = true;

    // Scrolling buildings
    struct Building {
        float x, y, w, h, speed;
        int   colorVal;
    };
    std::vector<Building> buildings;

    // Floating particles
    struct Particle {
        float x, y, speed, alpha, size;
    };
    std::vector<Particle> particles;

    void spawnBuildings();
    void spawnParticles();
    void drawBackground();
    void drawBuildings();
    void drawParticles();
    void drawTitle();
    void drawMenuItems();
    void drawFooter();
    void drawFade();
};