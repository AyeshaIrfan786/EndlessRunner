#include "Background.h"
#include <cstdlib>
#include <cstdint>

Background::Background() {}

void Background::update(float dt, float gameSpeed) {
    for (int i = 0; i < 3; i++) {
        offsets[i] += speeds[i] * gameSpeed * 300.f * dt;
        if (offsets[i] >= 800.f) offsets[i] -= 800.f;
    }
}

void Background::draw(sf::RenderWindow& window) {
    // Layer 0 — far buildings
    for (int pass = 0; pass < 2; pass++) {
        float base = -offsets[0] + pass * 800.f;
        int cols[]    = {40,  130, 260, 400, 540, 660};
        int heights[] = {200, 260, 180, 240, 190, 220};
        int widths[]  = {55,  60,  50,  65,  52,  58};
        for (int i = 0; i < 6; i++) {
            sf::RectangleShape bld({(float)widths[i], (float)heights[i]});
            bld.setPosition({base + cols[i], 420.f - heights[i]});
            bld.setFillColor(sf::Color(14, 14, 38));
            window.draw(bld);
            // Windows
            for (float wy = base + cols[i] + 6.f;
                 wy < base + cols[i] + widths[i] - 10.f; wy += 16.f) {
                for (int r = 0; r < 6; r++) {
                    if (rand() % 3 != 0) {
                        sf::RectangleShape win({7.f, 5.f});
                        win.setPosition({wy, 420.f - heights[i] + 14.f + r * 28.f});
                        win.setFillColor(rand() % 4 == 0
                            ? sf::Color(0, 160, 140, 100)
                            : sf::Color(30, 30, 70, 90));
                        window.draw(win);
                    }
                }
            }
        }
    }

    // Layer 1 — mid poles
    for (int pass = 0; pass < 2; pass++) {
        float base = -offsets[1] + pass * 800.f;
        int cols[] = {60, 220, 400, 580, 740};
        for (int i = 0; i < 5; i++) {
            sf::RectangleShape pole({4.f, 90.f});
            pole.setPosition({base + cols[i], 330.f});
            pole.setFillColor(sf::Color(0, 160, 140, 130));
            window.draw(pole);

            sf::RectangleShape arm({50.f, 3.f});
            arm.setPosition({base + cols[i] - 16.f, 330.f});
            arm.setFillColor(sf::Color(0, 180, 160, 100));
            window.draw(arm);
        }
    }

    // Layer 2 — near grid lines
    for (int pass = 0; pass < 2; pass++) {
        float base = -offsets[2] + pass * 800.f;
        for (int i = 0; i < 6; i++) {
            sf::RectangleShape line({2.f, 50.f});
            line.setPosition({base + i * 140.f, 420.f});
            line.setFillColor(sf::Color(0, 255, 200, 45));
            window.draw(line);
        }
    }
}