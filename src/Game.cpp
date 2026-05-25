#include "Game.h"
#include <cstdio>

Game::Game()
    : window(sf::VideoMode({800, 600}), "Endless Cyber Runner") {
    window.setFramerateLimit(60);
    if (!font.openFromFile("assets/font.ttf")) {}
    splash   = new SplashScreen(window, font);
    menu     = new MenuScreen(window, font);
    select   = new CharacterSelectScreen(window, font);
    game     = new GameScreen(window, font);
    gameover = new GameOverScreen(window, font);
}

Game::~Game() {
    delete splash;
    delete menu;
    delete select;
    delete game;
    delete gameover;
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;
        handleEvents();
        update(dt);
        draw();
    }
}

void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (state == GameState::CHARACTER_SELECT) {
                if (key->code == sf::Keyboard::Key::Enter ||
                    key->code == sf::Keyboard::Key::Space) {
                    int sel = select->getSelectedCharacter();
                    if (sel == 0) {
                        lastCharacter = {"SPEEDER","","Double Jump",2,1.4f,
                            sf::Color(0,200,255), sf::Color(0,255,200)};
                    } else if (sel == 1) {
                        lastCharacter = {"TANK","","Shield Bash",5,0.8f,
                            sf::Color(255,120,0), sf::Color(255,200,0)};
                    } else {
                        lastCharacter = {"HACKER","","Coin Magnet",3,1.0f,
                            sf::Color(180,0,255), sf::Color(220,100,255)};
                    }
                    game->init(lastCharacter);
                    state = GameState::PLAYING;
                }
                else if (key->code == sf::Keyboard::Key::Left)
                    select->handleInput(key->code, state);
                else if (key->code == sf::Keyboard::Key::Right)
                    select->handleInput(key->code, state);
                else if (key->code == sf::Keyboard::Key::Escape)
                    state = GameState::MENU;
            }
            else if (state == GameState::SPLASH)
                splash->handleInput(key->code, state);
            else if (state == GameState::MENU)
                menu->handleInput(key->code, state);
            else if (state == GameState::PLAYING)
                game->handleInput(key->code, state);
            else if (state == GameState::GAME_OVER)
                gameover->handleInput(key->code, state);
        }
    }
}

void Game::update(float dt) {
    // Reset select when entering from menu
    if (prevState == GameState::MENU &&
        state      == GameState::CHARACTER_SELECT)
        select->reset();

    // Init game over screen
    if (prevState == GameState::PLAYING &&
        state      == GameState::GAME_OVER) {
        gameover->init(
            game->getScore(),
            game->getHighScore(),
            lastCharacter.name);
    }

    // Restart — go back to character select
    if (prevState == GameState::GAME_OVER &&
        state      == GameState::CHARACTER_SELECT) {
        select->reset();
    }

    prevState = state;

    if      (state == GameState::SPLASH)
        splash->update(dt, state);
    else if (state == GameState::MENU)
        menu->update(dt);
    else if (state == GameState::CHARACTER_SELECT)
        select->update(dt);
    else if (state == GameState::PLAYING)
        game->update(dt, state);
    else if (state == GameState::GAME_OVER)
        gameover->update(dt);
}

void Game::draw() {
    if      (state == GameState::SPLASH)
        splash->draw();
    else if (state == GameState::MENU)
        menu->draw();
    else if (state == GameState::CHARACTER_SELECT)
        select->draw();
    else if (state == GameState::PLAYING)
        game->draw();
    else if (state == GameState::GAME_OVER)
        gameover->draw();

    window.display();
}