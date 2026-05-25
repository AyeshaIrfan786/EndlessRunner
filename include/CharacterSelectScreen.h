#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include <vector>
#include <string>

struct CharacterData {
    std::string name;
    std::string description;
    std::string ability;
    int         hearts;
    float       speed;
    sf::Color   bodyColor;
    sf::Color   accentColor;
};

class CharacterSelectScreen {
public:
    CharacterSelectScreen(sf::RenderWindow& window, sf::Font& font);
    void handleInput(sf::Keyboard::Key key, GameState& state);
    void update(float dt);
    void draw();
    int  getSelectedCharacter() const { return selected; }
    void reset() { fadingIn = true; fadeAlpha = 0.f; }

private:
    sf::RenderWindow& window;
    sf::Font&         font;

    int   selected   = 0;
    float fadeAlpha  = 0.f;
    bool  fadingIn   = true;
    float animTimer  = 0.f;
    float previewBob = 0.f;

    std::vector<CharacterData> characters;

    void setupCharacters();
    void drawBackground();
    void drawCards();
    void drawCard(int index, float x, float y, float w, float h, bool sel);
    void drawCharacterPreview(const CharacterData& c, float cx, float cy, bool sel);
    void drawStats(const CharacterData& c, float x, float y);
    void drawHearts(int count, float x, float y);
    void drawFooter();
    void drawFade();
};