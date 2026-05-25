#include "MenuScreen.h"
#include <cmath>
#include <cstdlib>
#include <cstdint>

MenuScreen::MenuScreen(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font) {
    spawnBuildings();
    spawnParticles();
}

void MenuScreen::spawnBuildings() {
    // Far layer
    for (int i = 0; i < 10; i++) {
        Building b;
        b.x        = i * 90.f;
        b.w        = 60.f + rand() % 50;
        b.h        = 160.f + rand() % 180;
        b.y        = 600.f - b.h;
        b.speed    = 18.f;
        b.colorVal = 12 + rand() % 10;
        buildings.push_back(b);
    }
    // Near layer
    for (int i = 0; i < 12; i++) {
        Building b;
        b.x        = i * 75.f;
        b.w        = 35.f + rand() % 30;
        b.h        = 70.f + rand() % 100;
        b.y        = 600.f - b.h;
        b.speed    = 40.f;
        b.colorVal = 8 + rand() % 8;
        buildings.push_back(b);
    }
}

void MenuScreen::spawnParticles() {
    for (int i = 0; i < 60; i++) {
        Particle p;
        p.x     = (float)(rand() % 800);
        p.y     = (float)(rand() % 500);
        p.speed = 10.f + rand() % 30;
        p.alpha = 40.f + rand() % 160;
        p.size  = 1.f + (rand() % 3);
        particles.push_back(p);
    }
}

void MenuScreen::handleInput(sf::Keyboard::Key key, GameState& state) {
    if (fadingIn) return;

    if (key == sf::Keyboard::Key::Up)
        selectedItem = (selectedItem - 1 + items.size()) % items.size();

    if (key == sf::Keyboard::Key::Down)
        selectedItem = (selectedItem + 1) % items.size();

    if (key == sf::Keyboard::Key::Enter ||
        key == sf::Keyboard::Key::Space) {
        if (selectedItem == 0) state = GameState::CHARACTER_SELECT;
        if (selectedItem == 1) state = GameState::CHARACTER_SELECT;
        if (selectedItem == 2) window.close();
    }
}

void MenuScreen::update(float dt) {
    pulseTimer += dt * 2.5f;
    scanline   += dt * 80.f;
    titleBob   += dt * 1.8f;
    if (scanline > 4.f) scanline -= 4.f;

    // Fade in
    if (fadingIn) {
        fadeAlpha += dt * 300.f;
        if (fadeAlpha >= 255.f) {
            fadeAlpha = 255.f;
            fadingIn  = false;
        }
    }

    // Scroll buildings
    for (auto& b : buildings) {
        b.x -= b.speed * dt;
        if (b.x + b.w < 0.f) {
            b.x        = 820.f;
            b.w        = 35.f + rand() % 70;
            b.h        = 70.f + rand() % 200;
            b.y        = 600.f - b.h;
            b.colorVal = 8 + rand() % 12;
        }
    }

    // Float particles upward
    for (auto& p : particles) {
        p.y -= p.speed * dt;
        if (p.y < -5.f) {
            p.y = 600.f;
            p.x = (float)(rand() % 800);
        }
    }
}

void MenuScreen::draw() {
    window.clear(sf::Color(4, 4, 18));
    drawBackground();
    drawBuildings();
    drawParticles();
    drawTitle();
    drawMenuItems();
    drawFooter();
    drawFade();
}

void MenuScreen::drawBackground() {
    // Horizon glow strip
    sf::RectangleShape glow({800.f, 60.f});
    glow.setPosition({0.f, 420.f});
    glow.setFillColor(sf::Color(0, 60, 50, 60));
    window.draw(glow);

    // Ground line
    sf::RectangleShape ground({800.f, 2.f});
    ground.setPosition({0.f, 478.f});
    ground.setFillColor(sf::Color(0, 255, 200, 160));
    window.draw(ground);

    sf::RectangleShape groundGlow({800.f, 10.f});
    groundGlow.setPosition({0.f, 479.f});
    groundGlow.setFillColor(sf::Color(0, 255, 200, 25));
    window.draw(groundGlow);

    // Scanlines
    for (float y = scanline; y < 600.f; y += 4.f) {
        sf::RectangleShape line({800.f, 1.f});
        line.setPosition({0.f, y});
        line.setFillColor(sf::Color(0, 0, 0, 20));
        window.draw(line);
    }
}

void MenuScreen::drawBuildings() {
    for (auto& b : buildings) {
        int c = b.colorVal;
        sf::RectangleShape rect({b.w, b.h});
        rect.setPosition({b.x, b.y});
        rect.setFillColor(sf::Color(c, c, c * 2));
        window.draw(rect);

        // Windows
        for (float wy = b.y + 8.f; wy < 478.f - 14.f; wy += 18.f) {
            for (float wx = b.x + 6.f; wx < b.x + b.w - 10.f; wx += 14.f) {
                if (rand() % 5 != 0) {
                    sf::RectangleShape win({6.f, 5.f});
                    win.setPosition({wx, wy});
                    bool cyan = rand() % 3 == 0;
                    win.setFillColor(cyan
                        ? sf::Color(0, 180, 160, 100)
                        : sf::Color(30, 30, 80, 90));
                    window.draw(win);
                }
            }
        }

        // Rooftop antenna
        sf::RectangleShape ant({2.f, 12.f});
        ant.setPosition({b.x + b.w / 2.f, b.y - 12.f});
        ant.setFillColor(sf::Color(0, 200, 160, 120));
        window.draw(ant);
    }
}

void MenuScreen::drawParticles() {
    for (auto& p : particles) {
        sf::RectangleShape dot({p.size, p.size});
        dot.setPosition({p.x, p.y});
        dot.setFillColor(sf::Color(0, 220, 180, (uint8_t)p.alpha));
        window.draw(dot);
    }
}

void MenuScreen::drawTitle() {
    float bob = std::sin(titleBob) * 4.f;

    // Glow shadow layers
    for (int i = 2; i >= 1; i--) {
        sf::Text g1(font, "ENDLESS", 74);
        g1.setFillColor(sf::Color(0, 200, 160, 25 * i));
        g1.setPosition({52.f - i * 3.f, 38.f + bob - i * 2.f});
        window.draw(g1);

        sf::Text g2(font, "CYBER RUNNER", 40);
        g2.setFillColor(sf::Color(0, 200, 160, 25 * i));
        g2.setPosition({52.f - i * 3.f, 118.f + bob - i * 2.f});
        window.draw(g2);
    }

    // Main title
    sf::Text title(font, "ENDLESS", 74);
    title.setFillColor(sf::Color(0, 255, 210));
    title.setPosition({52.f, 38.f + bob});
    window.draw(title);

    sf::Text sub(font, "CYBER RUNNER", 40);
    sub.setFillColor(sf::Color(220, 220, 220));
    sub.setPosition({52.f, 118.f + bob});
    window.draw(sub);

    // Accent line
    sf::RectangleShape line({500.f, 2.f});
    line.setPosition({52.f, 168.f + bob});
    line.setFillColor(sf::Color(0, 255, 200, 180));
    window.draw(line);
}

void MenuScreen::drawMenuItems() {
    float startY = 210.f;
    float gapY   = 64.f;

    for (int i = 0; i < (int)items.size(); i++) {
        bool  sel   = (i == selectedItem);
        float pulse = sel ? std::sin(pulseTimer) * 3.f : 0.f;
        float itemY = startY + i * gapY + pulse;

        if (sel) {
            // Selection background bar
            sf::RectangleShape bar({380.f, 48.f});
            bar.setPosition({44.f, itemY - 4.f});
            bar.setFillColor(sf::Color(0, 180, 140, 28));
            window.draw(bar);

            // Left accent
            sf::RectangleShape accent({4.f, 48.f});
            accent.setPosition({44.f, itemY - 4.f});
            accent.setFillColor(sf::Color(0, 255, 200));
            window.draw(accent);

            // Right arrow
            sf::Text arrow(font, ">", 26);
            arrow.setFillColor(sf::Color(0, 255, 200));
            arrow.setPosition({54.f, itemY});
            window.draw(arrow);
        }

        // Item text
        sf::Text text(font, items[i], 28);
        text.setFillColor(sel
            ? sf::Color(0, 255, 200)
            : sf::Color(100, 100, 130));
        text.setPosition({88.f, itemY});
        window.draw(text);

        // Subtle divider under each item
        if (!sel) {
            sf::RectangleShape div({380.f, 1.f});
            div.setPosition({88.f, itemY + 38.f});
            div.setFillColor(sf::Color(40, 40, 60));
            window.draw(div);
        }
    }
}

void MenuScreen::drawFooter() {
    sf::Text footer(font, "UP / DOWN    ENTER to select", 15);
    footer.setFillColor(sf::Color(55, 55, 75));
    footer.setPosition({44.f, 566.f});
    window.draw(footer);

    sf::Text ver(font, "v1.0  |  C++  SFML", 14);
    ver.setFillColor(sf::Color(40, 40, 60));
    ver.setPosition({620.f, 568.f});
    window.draw(ver);
}

void MenuScreen::drawFade() {
    if (fadeAlpha >= 255.f) return;
    uint8_t a = (uint8_t)(255.f - fadeAlpha);
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, a));
    window.draw(overlay);
}