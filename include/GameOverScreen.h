#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

class GameOverScreen {
public:
    GameOverScreen(sf::RenderWindow& window, sf::Font& font);
    void init(int score, int highScore, std::string characterName);
    void handleInput(sf::Keyboard::Key key, GameState& state);
    void update(float dt);
    void draw();

private:
    sf::RenderWindow& window;
    sf::Font&         font;

    int         finalScore  = 0;
    int         highScore   = 0;
    bool        isNewBest   = false;
    std::string charName    = "";

    float fadeAlpha   = 0.f;
    float animTimer   = 0.f;
    float pulseTimer  = 0.f;
    bool  fadingIn    = true;

    int   selectedItem = 0;

    // Particles
    struct Particle {
        float x, y, vx, vy, life, maxLife;
        sf::Color color;
    };
    std::vector<Particle> particles;

    void spawnParticles();
    void drawBackground();
    void drawTitle();
    void drawScore();
    void drawMenu();
    void drawParticles();
    void drawFade();
};