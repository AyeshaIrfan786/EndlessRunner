#include "GameOverScreen.h"
#include <cmath>
#include <cstdlib>
#include <cstdint>

GameOverScreen::GameOverScreen(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font) {}

void GameOverScreen::init(int score, int highScore, std::string characterName) {
    finalScore  = score;
    this->highScore = highScore;
    charName    = characterName;
    isNewBest   = (score >= highScore && score > 0);
    fadeAlpha   = 0.f;
    fadingIn    = true;
    animTimer   = 0.f;
    pulseTimer  = 0.f;
    selectedItem = 0;
    particles.clear();
    if (isNewBest) spawnParticles();
}

void GameOverScreen::spawnParticles() {
    for (int i = 0; i < 80; i++) {
        Particle p;
        p.x      = (float)(rand() % 800);
        p.y      = (float)(rand() % 600);
        p.vx     = (rand() % 60 - 30) / 10.f;
        p.vy     = -(rand() % 40 + 10) / 10.f;
        p.life   = p.maxLife = 1.5f + (rand() % 20) / 10.f;
        int r    = rand() % 3;
        p.color  = r == 0 ? sf::Color(255, 220, 0)
                 : r == 1 ? sf::Color(0, 255, 200)
                 :           sf::Color(255, 80, 80);
        particles.push_back(p);
    }
}

void GameOverScreen::handleInput(sf::Keyboard::Key key, GameState& state) {
    if (fadingIn) return;

    if (key == sf::Keyboard::Key::Up)
        selectedItem = (selectedItem - 1 + 2) % 2;
    if (key == sf::Keyboard::Key::Down)
        selectedItem = (selectedItem + 1) % 2;

    if (key == sf::Keyboard::Key::Enter ||
        key == sf::Keyboard::Key::Space) {
        if (selectedItem == 0)
            state = GameState::CHARACTER_SELECT;
        else
            state = GameState::MENU;
    }
    if (key == sf::Keyboard::Key::R)
        state = GameState::CHARACTER_SELECT;
    if (key == sf::Keyboard::Key::M)
        state = GameState::MENU;
}

void GameOverScreen::update(float dt) {
    animTimer  += dt;
    pulseTimer += dt * 2.5f;

    if (fadingIn) {
        fadeAlpha += dt * 280.f;
        if (fadeAlpha >= 255.f) {
            fadeAlpha = 255.f;
            fadingIn  = false;
        }
    }

    // Update particles
    for (auto& p : particles) {
        p.life -= dt;
        p.x    += p.vx;
        p.y    += p.vy;
        p.vy   += 0.05f;
        if (p.y > 620.f) {
            p.y    = -10.f;
            p.x    = (float)(rand() % 800);
            p.life = p.maxLife;
        }
    }
}

void GameOverScreen::draw() {
    window.clear(sf::Color(4, 4, 18));
    drawBackground();
    drawParticles();
    drawTitle();
    drawScore();
    drawMenu();
    drawFade();
}

void GameOverScreen::drawBackground() {
    // Dark overlay
    sf::RectangleShape bg({800.f, 600.f});
    bg.setFillColor(sf::Color(4, 4, 18));
    window.draw(bg);

    // Scanlines
    for (float y = 0.f; y < 600.f; y += 4.f) {
        sf::RectangleShape line({800.f, 1.f});
        line.setPosition({0.f, y});
        line.setFillColor(sf::Color(0, 0, 0, 20));
        window.draw(line);
    }

    // Bottom ground glow
    sf::RectangleShape glow({800.f, 3.f});
    glow.setPosition({0.f, 560.f});
    glow.setFillColor(sf::Color(0, 255, 200, 80));
    window.draw(glow);
}

void GameOverScreen::drawParticles() {
    for (auto& p : particles) {
        float t = p.life / p.maxLife;
        sf::RectangleShape dot({4.f * t + 1.f, 4.f * t + 1.f});
        dot.setPosition({p.x, p.y});
        sf::Color c = p.color;
        c.a = (uint8_t)(200 * t);
        dot.setFillColor(c);
        window.draw(dot);
    }
}

void GameOverScreen::drawTitle() {
    float shake = (animTimer < 0.5f)
        ? std::sin(animTimer * 40.f) * (1.f - animTimer * 2.f) * 8.f
        : 0.f;

    // Glow
    for (int i = 2; i >= 1; i--) {
        sf::Text g(font, "GAME OVER", 72);
        g.setFillColor(sf::Color(220, 40, 40, 30 * i));
        g.setPosition({400.f - g.getLocalBounds().size.x/2.f - i*3.f,
                       80.f + shake - i*2.f});
        window.draw(g);
    }

    sf::Text title(font, "GAME OVER", 72);
    title.setFillColor(sf::Color(220, 40, 40));
    title.setPosition({400.f - title.getLocalBounds().size.x/2.f, 80.f + shake});
    window.draw(title);

    // Character name tag
    if (!charName.empty()) {
        sf::Text ct(font, "Playing as: " + charName, 18);
        ct.setFillColor(sf::Color(80, 80, 100));
        ct.setPosition({400.f - ct.getLocalBounds().size.x/2.f, 166.f});
        window.draw(ct);
    }

    // Divider
    sf::RectangleShape div({400.f, 2.f});
    div.setPosition({200.f, 196.f});
    div.setFillColor(sf::Color(50, 50, 70));
    window.draw(div);
}

void GameOverScreen::drawScore() {
    // Score box
    sf::RectangleShape box({360.f, 120.f});
    box.setPosition({220.f, 210.f});
    box.setFillColor(sf::Color(0, 20, 18));
    window.draw(box);

    sf::RectangleShape boxBorder({360.f, 2.f});
    boxBorder.setPosition({220.f, 210.f});
    boxBorder.setFillColor(sf::Color(0, 255, 200, 80));
    window.draw(boxBorder);

    sf::RectangleShape boxBorderB({360.f, 2.f});
    boxBorderB.setPosition({220.f, 328.f});
    boxBorderB.setFillColor(sf::Color(0, 255, 200, 80));
    window.draw(boxBorderB);

    // Score label
    sf::Text sLabel(font, "SCORE", 16);
    sLabel.setFillColor(sf::Color(80, 80, 100));
    sLabel.setPosition({240.f, 224.f});
    window.draw(sLabel);

    sf::Text sVal(font, std::to_string(finalScore), 48);
    sVal.setFillColor(sf::Color::White);
    sVal.setPosition({240.f, 244.f});
    window.draw(sVal);

    // Best label
    sf::Text bLabel(font, "BEST", 16);
    bLabel.setFillColor(sf::Color(80, 80, 100));
    bLabel.setPosition({460.f, 224.f});
    window.draw(bLabel);

    sf::Text bVal(font, std::to_string(highScore), 48);
    bVal.setFillColor(sf::Color(0, 220, 180));
    bVal.setPosition({460.f, 244.f});
    window.draw(bVal);

    // New best banner
    if (isNewBest) {
        float pulse = std::sin(pulseTimer) * 0.15f + 0.85f;
        sf::Text nb(font, "NEW BEST!", 22);
        nb.setFillColor(sf::Color(255, 220, 0,
            (uint8_t)(255 * pulse)));
        nb.setPosition({400.f - nb.getLocalBounds().size.x/2.f, 296.f});
        window.draw(nb);
    }
}

void GameOverScreen::drawMenu() {
    std::string items[2] = {"PLAY AGAIN", "MAIN MENU"};
    float startY = 360.f;
    float gapY   = 60.f;

    for (int i = 0; i < 2; i++) {
        bool  sel   = (i == selectedItem);
        float pulse = sel ? std::sin(pulseTimer) * 3.f : 0.f;
        float itemY = startY + i * gapY + pulse;

        if (sel) {
            sf::RectangleShape bar({320.f, 44.f});
            bar.setPosition({240.f, itemY - 4.f});
            bar.setFillColor(sf::Color(0, 180, 140, 28));
            window.draw(bar);

            sf::RectangleShape accent({4.f, 44.f});
            accent.setPosition({240.f, itemY - 4.f});
            accent.setFillColor(sf::Color(0, 255, 200));
            window.draw(accent);

            sf::Text arrow(font, ">", 24);
            arrow.setFillColor(sf::Color(0, 255, 200));
            arrow.setPosition({250.f, itemY});
            window.draw(arrow);
        }

        sf::Text item(font, items[i], 26);
        item.setFillColor(sel
            ? sf::Color(0, 255, 200)
            : sf::Color(80, 80, 100));
        item.setPosition({278.f, itemY});
        window.draw(item);
    }

    // Footer hints
    sf::Text hint(font, "R  play again        M  main menu", 14);
    hint.setFillColor(sf::Color(40, 40, 60));
    hint.setPosition({400.f - hint.getLocalBounds().size.x/2.f, 500.f});
    window.draw(hint);
}

void GameOverScreen::drawFade() {
    if (fadeAlpha >= 255.f) return;
    uint8_t a = (uint8_t)(255.f - fadeAlpha);
    sf::RectangleShape o({800.f, 600.f});
    o.setFillColor(sf::Color(0, 0, 0, a));
    window.draw(o);
}