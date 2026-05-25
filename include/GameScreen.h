#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.h"
#include "Player.h"
#include "StaticObstacle.h"
#include "DynamicObstacle.h"
#include "FlyingObstacle.h"
#include "Coin.h"
#include "Background.h"
#include "Particle.h"
#include "FileHandler.h"
#include "CharacterSelectScreen.h"

class GameScreen {
public:
    GameScreen(sf::RenderWindow& window, sf::Font& font);
    void init(const CharacterData& data);
    void handleInput(sf::Keyboard::Key key, GameState& state);
    void update(float dt, GameState& state);
    void draw();
    void reset(const CharacterData& data);
    int  getScore()     const { return score; }
    int  getHighScore() const { return highScore; }

private:
    sf::RenderWindow& window;
    sf::Font&         font;

    Player         player;
    Background     background;
    ParticleSystem particles;

    std::vector<Entity*> obstacles;
    std::vector<Coin*>   coins;

    int   score        = 0;
    int   highScore    = 0;
    float scoreTimer   = 0.f;
    float spawnTimer   = 0.f;
    float spawnInterval = 2.f;
    float gameSpeed    = 1.f;

    float shakeTimer   = 0.f;
    float shakeMag     = 0.f;
    float flashTimer   = 0.f;

    sf::Text* scoreTxt   = nullptr;
    sf::Text* hiScoreTxt = nullptr;
    sf::Text* speedTxt   = nullptr;

    bool paused = false;

    void spawnObstacle();
    void spawnCoin();
    void checkCollisions(GameState& state);
    void cleanup();
    void triggerShake(float mag, float dur);
    void drawHUD();
    void drawLanes();
    void drawGround();
    void drawHearts();
    void drawPauseOverlay();
};