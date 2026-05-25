#include "CharacterSelectScreen.h"
#include <cmath>
#include <cstdint>

CharacterSelectScreen::CharacterSelectScreen(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font) {
    setupCharacters();
}

void CharacterSelectScreen::setupCharacters() {
    CharacterData c1;
    c1.name        = "SPEEDER";
    c1.description = "Lightning fast";
    c1.ability     = "Double Jump";
    c1.hearts      = 2;
    c1.speed       = 1.4f;
    c1.bodyColor   = sf::Color(0, 200, 255);
    c1.accentColor = sf::Color(0, 255, 200);
    characters.push_back(c1);

    CharacterData c2;
    c2.name        = "TANK";
    c2.description = "Heavy & tough";
    c2.ability     = "Shield Bash";
    c2.hearts      = 5;
    c2.speed       = 0.8f;
    c2.bodyColor   = sf::Color(255, 120, 0);
    c2.accentColor = sf::Color(255, 200, 0);
    characters.push_back(c2);

    CharacterData c3;
    c3.name        = "HACKER";
    c3.description = "Balanced warrior";
    c3.ability     = "Coin Magnet";
    c3.hearts      = 3;
    c3.speed       = 1.0f;
    c3.bodyColor   = sf::Color(180, 0, 255);
    c3.accentColor = sf::Color(220, 100, 255);
    characters.push_back(c3);
}

void CharacterSelectScreen::handleInput(sf::Keyboard::Key key, GameState& state) {
    if (key == sf::Keyboard::Key::Left)
        selected = (selected - 1 + characters.size()) % characters.size();
    if (key == sf::Keyboard::Key::Right)
        selected = (selected + 1) % characters.size();
    if (key == sf::Keyboard::Key::Enter ||
        key == sf::Keyboard::Key::Space)
        state = GameState::PLAYING;
    if (key == sf::Keyboard::Key::Escape)
        state = GameState::MENU;
}

void CharacterSelectScreen::update(float dt) {
    animTimer  += dt * 3.f;
    previewBob += dt * 2.f;
    if (fadingIn) {
        fadeAlpha += dt * 320.f;
        if (fadeAlpha >= 255.f) {
            fadeAlpha = 255.f;
            fadingIn  = false;
        }
    }
}

void CharacterSelectScreen::draw() {
    window.clear(sf::Color(4, 4, 18));
    drawBackground();
    drawCards();
    drawFooter();
    drawFade();
}

void CharacterSelectScreen::drawBackground() {
    sf::RectangleShape header({800.f, 72.f});
    header.setFillColor(sf::Color(0, 20, 18));
    window.draw(header);

    sf::RectangleShape headerLine({800.f, 2.f});
    headerLine.setPosition({0.f, 72.f});
    headerLine.setFillColor(sf::Color(0, 255, 200, 120));
    window.draw(headerLine);

    sf::Text title(font, "SELECT YOUR RUNNER", 30);
    title.setFillColor(sf::Color(0, 255, 200));
    title.setPosition({400.f - title.getLocalBounds().size.x / 2.f, 20.f});
    window.draw(title);

    for (int x = 0; x < 800; x += 44) {
        for (int y = 80; y < 600; y += 44) {
            sf::RectangleShape dot({2.f, 2.f});
            dot.setPosition({(float)x, (float)y});
            dot.setFillColor(sf::Color(0, 80, 60, 35));
            window.draw(dot);
        }
    }
}

void CharacterSelectScreen::drawCards() {
    float cardW  = 245.f;
    float cardH  = 450.f;
    float gap    = 12.f;
    float totalW = 3 * cardW + 2 * gap;
    float startX = (800.f - totalW) / 2.f;
    float startY = 78.f;

    for (int i = 0; i < (int)characters.size(); i++) {
        float x     = startX + i * (cardW + gap);
        bool  sel   = (i == selected);
        float cardY = sel ? startY - 12.f : startY;
        drawCard(i, x, cardY, cardW, cardH, sel);
    }
}

void CharacterSelectScreen::drawCard(int index, float x, float y,
                                      float w, float h, bool sel) {
    const CharacterData& c = characters[index];

    sf::RectangleShape card({w, h});
    card.setPosition({x, y});
    card.setFillColor(sel ? sf::Color(0, 28, 24) : sf::Color(8, 8, 20));
    window.draw(card);

    sf::Color bc = sel ? c.accentColor : sf::Color(28, 28, 48);
    auto drawBorder = [&](float bx, float by, float bw, float bh) {
        sf::RectangleShape b({bw, bh});
        b.setPosition({bx, by});
        b.setFillColor(bc);
        window.draw(b);
    };
    drawBorder(x,       y,       w,   2.f);
    drawBorder(x,       y+h-2.f, w,   2.f);
    drawBorder(x,       y,       2.f, h);
    drawBorder(x+w-2.f, y,       2.f, h);

    if (sel) {
        float cs = 14.f;
        drawBorder(x,       y,       cs,  2.f);
        drawBorder(x,       y,       2.f, cs);
        drawBorder(x+w-cs,  y,       cs,  2.f);
        drawBorder(x+w-2.f, y,       2.f, cs);
        drawBorder(x,       y+h-2.f, cs,  2.f);
        drawBorder(x,       y+h-cs,  2.f, cs);
        drawBorder(x+w-cs,  y+h-2.f, cs,  2.f);
        drawBorder(x+w-2.f, y+h-cs,  2.f, cs);

        sf::RectangleShape tag({w, 22.f});
        tag.setPosition({x, y});
        tag.setFillColor(sf::Color(c.accentColor.r, c.accentColor.g, c.accentColor.b, 45));
        window.draw(tag);

        sf::Text selTxt(font, "SELECTED", 12);
        selTxt.setFillColor(c.accentColor);
        selTxt.setPosition({x + w/2.f - selTxt.getLocalBounds().size.x/2.f, y + 4.f});
        window.draw(selTxt);
    }

    float cx = x + w / 2.f;
    float cy = y + (sel ? 158.f : 165.f);
    drawCharacterPreview(c, cx, cy, sel);

    // Name
    sf::Text name(font, c.name, 20);
    name.setFillColor(sel ? c.accentColor : sf::Color(150, 150, 170));
    name.setPosition({x + w/2.f - name.getLocalBounds().size.x/2.f, y + h - 142.f});
    window.draw(name);

    // Desc
    sf::Text desc(font, c.description, 12);
    desc.setFillColor(sf::Color(80, 80, 100));
    desc.setPosition({x + w/2.f - desc.getLocalBounds().size.x/2.f, y + h - 118.f});
    window.draw(desc);

    // Divider
    sf::RectangleShape div({w - 20.f, 1.f});
    div.setPosition({x + 10.f, y + h - 98.f});
    div.setFillColor(sf::Color(28, 28, 48));
    window.draw(div);

    // Stats
    drawStats(c, x + 12.f, y + h - 92.f);

    // Ability badge
    sf::RectangleShape abilBg({w - 20.f, 24.f});
    abilBg.setPosition({x + 10.f, y + h - 38.f});
    abilBg.setFillColor(sf::Color(c.accentColor.r, c.accentColor.g, c.accentColor.b, 22));
    window.draw(abilBg);

    sf::Text abil(font, c.ability, 12);
    abil.setFillColor(sel
        ? sf::Color(c.accentColor.r, c.accentColor.g, c.accentColor.b, 220)
        : sf::Color(60, 60, 80));
    abil.setPosition({x + w/2.f - abil.getLocalBounds().size.x/2.f, y + h - 34.f});
    window.draw(abil);
}

void CharacterSelectScreen::drawCharacterPreview(
    const CharacterData& c, float cx, float cy, bool sel) {

    float bob = sel ? std::sin(previewBob) * 5.f : 0.f;
    cy += bob;

    sf::RectangleShape shadow({44.f, 6.f});
    shadow.setPosition({cx - 22.f, cy + 52.f});
    shadow.setFillColor(sf::Color(0, 0, 0, 60));
    window.draw(shadow);

    sf::RectangleShape leg1({13.f, 20.f});
    leg1.setPosition({cx - 16.f, cy + 28.f});
    leg1.setFillColor(sf::Color(
        (uint8_t)(c.bodyColor.r * 0.65f),
        (uint8_t)(c.bodyColor.g * 0.65f),
        (uint8_t)(c.bodyColor.b * 0.65f)));
    window.draw(leg1);

    sf::RectangleShape leg2({13.f, 20.f});
    leg2.setPosition({cx + 3.f, cy + 28.f});
    leg2.setFillColor(leg1.getFillColor());
    window.draw(leg2);

    sf::RectangleShape body({38.f, 54.f});
    body.setPosition({cx - 19.f, cy - 26.f});
    body.setFillColor(c.bodyColor);
    window.draw(body);

    sf::RectangleShape stripe({38.f, 6.f});
    stripe.setPosition({cx - 19.f, cy - 6.f});
    stripe.setFillColor(sf::Color(c.accentColor.r, c.accentColor.g, c.accentColor.b, 120));
    window.draw(stripe);

    sf::RectangleShape head({30.f, 24.f});
    head.setPosition({cx - 15.f, cy - 54.f});
    head.setFillColor(sf::Color(
        (uint8_t)std::min(255, (int)c.bodyColor.r + 35),
        (uint8_t)std::min(255, (int)c.bodyColor.g + 35),
        (uint8_t)std::min(255, (int)c.bodyColor.b + 35)));
    window.draw(head);

    sf::RectangleShape visor({22.f, 7.f});
    visor.setPosition({cx - 11.f, cy - 47.f});
    visor.setFillColor(sf::Color(c.accentColor.r, c.accentColor.g, c.accentColor.b, 200));
    window.draw(visor);

    sf::RectangleShape ant({2.f, 10.f});
    ant.setPosition({cx - 1.f, cy - 64.f});
    ant.setFillColor(c.accentColor);
    window.draw(ant);

    sf::RectangleShape antTip({6.f, 6.f});
    antTip.setPosition({cx - 3.f, cy - 68.f});
    antTip.setFillColor(c.accentColor);
    window.draw(antTip);

    if (sel) {
        float glowA = (std::sin(animTimer) + 1.f) / 2.f * 50.f + 20.f;
        for (int r = 3; r >= 1; r--) {
            sf::RectangleShape glow({50.f + r * 10.f, 5.f});
            glow.setPosition({cx - 25.f - r*5.f, cy + 50.f});
            glow.setFillColor(sf::Color(
                c.accentColor.r, c.accentColor.g,
                c.accentColor.b, (uint8_t)(glowA / r)));
            window.draw(glow);
        }
    }
}

void CharacterSelectScreen::drawStats(const CharacterData& c, float x, float y) {
    sf::Text hLabel(font, "HP", 12);
    hLabel.setFillColor(sf::Color(70, 70, 90));
    hLabel.setPosition({x, y});
    window.draw(hLabel);
    drawHearts(c.hearts, x + 28.f, y + 2.f);

    sf::Text sLabel(font, "SP", 12);
    sLabel.setFillColor(sf::Color(70, 70, 90));
    sLabel.setPosition({x, y + 22.f});
    window.draw(sLabel);

    sf::RectangleShape bg({90.f, 6.f});
    bg.setPosition({x + 34.f, y + 27.f});
    bg.setFillColor(sf::Color(18, 18, 32));
    window.draw(bg);

    float fill = (c.speed / 1.5f) * 90.f;
    sf::RectangleShape bar({fill, 6.f});
    bar.setPosition({x + 34.f, y + 27.f});
    bar.setFillColor(c.accentColor);
    window.draw(bar);
}

void CharacterSelectScreen::drawHearts(int count, float x, float y) {
    for (int i = 0; i < 5; i++) {
        sf::RectangleShape h({10.f, 10.f});
        h.setPosition({x + i * 13.f, y});
        h.setFillColor(i < count
            ? sf::Color(220, 50, 80)
            : sf::Color(35, 35, 50));
        window.draw(h);
    }
}

void CharacterSelectScreen::drawFooter() {
    sf::Text f(font, "LEFT / RIGHT   browse        ENTER   confirm        ESC   back", 14);
    f.setFillColor(sf::Color(45, 45, 65));
    f.setPosition({400.f - f.getLocalBounds().size.x/2.f, 568.f});
    window.draw(f);
}

void CharacterSelectScreen::drawFade() {
    if (fadeAlpha >= 255.f) return;
    uint8_t a = (uint8_t)(255.f - fadeAlpha);
    sf::RectangleShape o({800.f, 600.f});
    o.setFillColor(sf::Color(0, 0, 0, a));
    window.draw(o);
}