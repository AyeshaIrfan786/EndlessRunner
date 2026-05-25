#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "SplashScreen.h"
#include "MenuScreen.h"
#include "CharacterSelectScreen.h"
#include "GameScreen.h"
#include "GameOverScreen.h"

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    sf::RenderWindow       window;
    sf::Font               font;
    GameState              state     = GameState::SPLASH;
    GameState              prevState = GameState::SPLASH;

    SplashScreen*          splash   = nullptr;
    MenuScreen*            menu     = nullptr;
    CharacterSelectScreen* select   = nullptr;
    GameScreen*            game     = nullptr;
    GameOverScreen*        gameover = nullptr;

    CharacterData lastCharacter;

    void handleEvents();
    void update(float dt);
    void draw();
};