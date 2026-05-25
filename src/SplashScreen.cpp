#include "SplashScreen.h"
#include <cmath>
#include <cstdlib>
#include <cstdint>

SplashScreen::SplashScreen(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font) {
    spawnStars();
}

void SplashScreen::spawnStars() {
    for (int i = 0; i < 200; i++) {
        Star s;
        s.x          = (float)(rand() % 800);
        s.y          = (float)(rand() % 600);
        s.size       = 0.5f + (rand() % 20) / 10.f;
        s.speed      = 0.2f + (rand() % 30) / 100.f;
        s.brightness = 80.f + rand() % 175;
        stars.push_back(s);
    }
}

void SplashScreen::handleInput(sf::Keyboard::Key key, GameState& state) {
    if (phase == 2) {
        anyKeyPressed = true;
        phase = 3;
        phaseTimer = 0.f;
    }
}

void SplashScreen::update(float dt, GameState& state) {
    phaseTimer   += dt;
    blinkTimer   += dt;
    groundOffset += 60.f * dt;
    if (groundOffset > 80.f) groundOffset -= 80.f;

    for (auto& s : stars) {
        s.x -= s.speed * dt * 20.f;
        if (s.x < 0.f) s.x = 800.f;
    }

    if (blinkTimer > 0.55f) {
        blinkOn    = !blinkOn;
        blinkTimer = 0.f;
    }

    switch (phase) {
        case 0:
            fadeAlpha = std::min(255.f, phaseTimer * 220.f);
            if (phaseTimer >= 1.2f) {
                phase      = 1;
                phaseTimer = 0.f;
                fadeAlpha  = 255.f;
            }
            break;
        case 1:
            if (phaseTimer >= 1.0f) {
                phase      = 2;
                phaseTimer = 0.f;
            }
            break;
        case 2:
            break;
        case 3:
            fadeAlpha = std::max(0.f, 255.f - phaseTimer * 400.f);
            if (phaseTimer >= 0.65f)
                state = GameState::MENU;
            break;
    }
}

void SplashScreen::draw() {
    window.clear(sf::Color(3, 3, 14));
    drawStars();
    drawGround();
    drawTitle();
    if (phase >= 2) drawPressAnyKey();
    drawFadeOverlay();
}

void SplashScreen::drawStars() {
    for (auto& s : stars) {
        uint8_t b  = (uint8_t)s.brightness;
        uint8_t b2 = (uint8_t)(s.brightness * 1.1f > 255.f ? 255.f : s.brightness * 1.1f);
        uint8_t a  = (uint8_t)fadeAlpha;

        sf::RectangleShape star({s.size, s.size});
        star.setPosition({s.x, s.y});
        star.setFillColor(sf::Color(b, b, b2, a));
        window.draw(star);
    }
}

void SplashScreen::drawGround() {
    uint8_t lineA = (uint8_t)(40.f * fadeAlpha / 255.f);
    sf::Color lineColor(0, 255, 200, lineA);

    float horizons[] = {460.f, 490.f, 515.f, 535.f, 550.f, 562.f, 572.f};
    for (float y : horizons) {
        sf::RectangleShape line({800.f, 1.f});
        line.setPosition({0.f, y});
        line.setFillColor(lineColor);
        window.draw(line);
    }

    float cx       = 400.f;
    float horizonY = 458.f;
    int   numLines = 12;
    for (int i = 0; i <= numLines; i++) {
        float botX = i * (800.f / numLines);
        sf::Vertex verts[2] = {
            sf::Vertex{{cx,   horizonY}, lineColor},
            sf::Vertex{{botX, 600.f},   lineColor}
        };
        window.draw(verts, 2, sf::PrimitiveType::Lines);
    }

    uint8_t groundA = (uint8_t)(120.f * fadeAlpha / 255.f);
    sf::RectangleShape ground({800.f, 145.f});
    ground.setPosition({0.f, 455.f});
    ground.setFillColor(sf::Color(0, 8, 6, groundA));
    window.draw(ground);
}

void SplashScreen::drawTitle() {
    uint8_t a = (uint8_t)fadeAlpha;

    float offsets[] = {4.f, 2.f};
    uint8_t glowA[] = {
        (uint8_t)(30.f  * a / 255.f),
        (uint8_t)(60.f  * a / 255.f)
    };

    for (int i = 0; i < 2; i++) {
        sf::Text glow1(font, "ENDLESS", 90);
        glow1.setFillColor(sf::Color(0, 220, 180, glowA[i]));
        glow1.setPosition({80.f - offsets[i], 130.f - offsets[i]});
        window.draw(glow1);

        sf::Text glow2(font, "CYBER RUNNER", 52);
        glow2.setFillColor(sf::Color(0, 220, 180, glowA[i]));
        glow2.setPosition({80.f - offsets[i], 228.f - offsets[i]});
        window.draw(glow2);
    }

    sf::Text title(font, "ENDLESS", 90);
    title.setFillColor(sf::Color(0, 255, 210, a));
    title.setPosition({80.f, 130.f});
    window.draw(title);

    sf::Text sub(font, "CYBER RUNNER", 52);
    sub.setFillColor(sf::Color(255, 255, 255, a));
    sub.setPosition({80.f, 228.f});
    window.draw(sub);

    sf::RectangleShape line({620.f, 2.f});
    line.setPosition({80.f, 296.f});
    line.setFillColor(sf::Color(0, 255, 200, (uint8_t)(160.f * a / 255.f)));
    window.draw(line);

  
}

void SplashScreen::drawPressAnyKey() {
    if (!blinkOn) return;

    uint8_t a = (uint8_t)(180.f * fadeAlpha / 255.f);

    sf::Text prompt(font, "PRESS ANY KEY TO START", 26);
    prompt.setFillColor(sf::Color(0, 255, 200, a));

    float tx = 400.f - prompt.getLocalBounds().size.x / 2.f;
    prompt.setPosition({tx, 400.f});
    window.draw(prompt);

    sf::RectangleShape ul({prompt.getLocalBounds().size.x, 2.f});
    ul.setPosition({tx, 432.f});
    ul.setFillColor(sf::Color(0, 255, 200, (uint8_t)(80.f * fadeAlpha / 255.f)));
    window.draw(ul);
}

void SplashScreen::drawFadeOverlay() {
    uint8_t overlayA = 0;

    if (phase == 0)
        overlayA = (uint8_t)(255.f - fadeAlpha);
    else if (phase == 3)
        overlayA = (uint8_t)(255.f - fadeAlpha);

    if (overlayA > 0) {
        sf::RectangleShape overlay({800.f, 600.f});
        overlay.setFillColor(sf::Color(0, 0, 0, overlayA));
        window.draw(overlay);
    }
}