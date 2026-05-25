#include "GameScreen.h"
#include <cstdlib>
#include <cmath>
#include <cstdint>

static const float GROUND_Y = 420.f;

GameScreen::GameScreen(sf::RenderWindow& window, sf::Font& font)
    : window(window), font(font) {

    highScore = FileHandler::loadHighScore();

    scoreTxt   = new sf::Text(font, "SCORE  0", 22);
    hiScoreTxt = new sf::Text(font, "BEST  0", 18);
    speedTxt   = new sf::Text(font, "SPD", 14);

    scoreTxt->setFillColor(sf::Color::White);
    scoreTxt->setPosition({20.f, 16.f});

    hiScoreTxt->setFillColor(sf::Color(0, 220, 180));
    hiScoreTxt->setPosition({20.f, 44.f});
    hiScoreTxt->setString("BEST  " + std::to_string(highScore));

    speedTxt->setFillColor(sf::Color(80, 80, 100));
    speedTxt->setPosition({700.f, 16.f});
}

void GameScreen::init(const CharacterData& data) {
    player = Player(data);
    cleanup();
    score         = 0;
    scoreTimer    = 0.f;
    spawnTimer    = 0.f;
    spawnInterval = 2.f;
    gameSpeed     = 1.f;
    paused        = false;
    scoreTxt->setString("SCORE  0");
}

void GameScreen::handleInput(sf::Keyboard::Key key, GameState& state) {
    if (key == sf::Keyboard::Key::P) {
        paused = !paused;
        return;
    }
    if (key == sf::Keyboard::Key::Escape) {
        state = GameState::MENU;
        return;
    }
    if (paused) return;

    if (key == sf::Keyboard::Key::Space ||
        key == sf::Keyboard::Key::Up    ||
        key == sf::Keyboard::Key::W)
        player.jump();

    if (key == sf::Keyboard::Key::Down ||
        key == sf::Keyboard::Key::S)
        player.slide();

    if (key == sf::Keyboard::Key::Left ||
        key == sf::Keyboard::Key::A)
        player.switchLane(-1);

    if (key == sf::Keyboard::Key::Right ||
        key == sf::Keyboard::Key::D)
        player.switchLane(1);
}

void GameScreen::update(float dt, GameState& state) {
    if (paused) return;
    if (player.state == PlayerState::DEAD) {
        state = GameState::GAME_OVER;
        return;
    }

    background.update(dt, gameSpeed);
    player.update(dt);
    particles.update(dt);

    if (shakeTimer > 0.f) shakeTimer -= dt;
    if (flashTimer > 0.f) flashTimer -= dt;

    scoreTimer += dt;
    if (scoreTimer >= 0.4f) {
        scoreTimer = 0.f;
        score++;
        scoreTxt->setString("SCORE  " + std::to_string(score));
        if (score % 10 == 0) {
            gameSpeed += 0.08f;
            flashTimer = 0.25f;
        }
    }

    gameSpeed = 1.f + score * 0.035f;

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnTimer    = 0.f;
        spawnInterval = 1.1f + (rand() % 18) / 10.f;
        spawnObstacle();
        if (score % 2 == 0) spawnCoin();
    }

    for (Entity* e : obstacles) e->update(dt, gameSpeed);
    for (Coin*   c : coins)     c->update(dt, gameSpeed);

    checkCollisions(state);

    for (int i = (int)obstacles.size()-1; i >= 0; i--) {
        if (obstacles[i]->isOffScreen()) {
            delete obstacles[i];
            obstacles.erase(obstacles.begin() + i);
        }
    }
    for (int i = (int)coins.size()-1; i >= 0; i--) {
        if (coins[i]->isOffScreen() || coins[i]->collected) {
            delete coins[i];
            coins.erase(coins.begin() + i);
        }
    }
}

void GameScreen::spawnObstacle() {
    int   lane = rand() % 3;
    float lx   = Player::laneX[lane];
    int   type = rand() % 3;
    if      (type == 0) obstacles.push_back(new StaticObstacle(lx));
    else if (type == 1) obstacles.push_back(new DynamicObstacle(lx));
    else                obstacles.push_back(new FlyingObstacle(lx));
}

void GameScreen::spawnCoin() {
    int   lane = rand() % 3;
    float lx   = Player::laneX[lane];
    float y    = 310.f + (rand() % 80);
    coins.push_back(new Coin(lx, y));
}

void GameScreen::checkCollisions(GameState& state) {
    for (Entity* e : obstacles) {
        if (!player.isInvincible() &&
            player.bounds.findIntersection(e->bounds)) {
            player.takeDamage();
            triggerShake(4.f, 0.4f);
            particles.emit(
                player.bounds.position.x + 20.f,
                player.bounds.position.y + 30.f,
                sf::Color(255, 80, 80), 18);
            if (score > highScore) {
                highScore = score;
                FileHandler::saveHighScore(highScore);
                hiScoreTxt->setString("BEST  " + std::to_string(highScore));
            }
        }
    }
    for (Coin* c : coins) {
        if (!c->collected &&
            player.bounds.findIntersection(c->bounds)) {
            c->collected = true;
            score += 3;
            scoreTxt->setString("SCORE  " + std::to_string(score));
            particles.emit(
                c->bounds.position.x + 10.f,
                c->bounds.position.y + 10.f,
                sf::Color(255, 220, 0), 10);
        }
    }
}

void GameScreen::triggerShake(float mag, float dur) {
    shakeMag   = mag;
    shakeTimer = dur;
}

void GameScreen::cleanup() {
    for (Entity* e : obstacles) delete e;
    obstacles.clear();
    for (Coin* c : coins) delete c;
    coins.clear();
}

void GameScreen::reset(const CharacterData& data) {
    init(data);
}

void GameScreen::draw() {
    sf::View view = window.getDefaultView();
    if (shakeTimer > 0.f) {
        float s = shakeMag * (shakeTimer / 0.4f);
        view.move({(float)(rand()%11-5) * s * 0.3f,
                   (float)(rand()%11-5) * s * 0.3f});
    }
    window.setView(view);

    window.clear(sf::Color(4, 4, 18));
    background.draw(window);
    drawGround();
    drawLanes();

    for (Entity* e : obstacles) e->draw(window);
    for (Coin*   c : coins)     c->draw(window);
    player.draw(window);
    particles.draw(window);

    if (flashTimer > 0.f) {
        sf::RectangleShape flash({800.f, 500.f});
        flash.setFillColor(sf::Color(0, 255, 200,
            (uint8_t)(70.f * flashTimer / 0.25f)));
        window.draw(flash);
    }

    window.setView(window.getDefaultView());
    drawHUD();

    if (paused) drawPauseOverlay();
}

void GameScreen::drawGround() {
    sf::RectangleShape g({800.f, 3.f});
    g.setPosition({0.f, GROUND_Y});
    g.setFillColor(sf::Color(0, 200, 160, 160));
    window.draw(g);

    sf::RectangleShape glow({800.f, 14.f});
    glow.setPosition({0.f, GROUND_Y + 2.f});
    glow.setFillColor(sf::Color(0, 200, 160, 22));
    window.draw(glow);
}

void GameScreen::drawLanes() {
    for (int i = 0; i < 3; i++) {
        sf::RectangleShape lane({1.f, GROUND_Y});
        lane.setPosition({Player::laneX[i], 0.f});
        lane.setFillColor(sf::Color(0, 255, 200, 14));
        window.draw(lane);
    }
}

void GameScreen::drawHUD() {
    sf::RectangleShape hudBg({800.f, 68.f});
    hudBg.setFillColor(sf::Color(0, 0, 0, 140));
    window.draw(hudBg);

    window.draw(*scoreTxt);
    window.draw(*hiScoreTxt);
    window.draw(*speedTxt);

    float speedFill = std::min((gameSpeed - 1.f) / 3.f, 1.f) * 80.f;
    sf::RectangleShape speedBg({80.f, 8.f});
    speedBg.setPosition({718.f, 36.f});
    speedBg.setFillColor(sf::Color(20, 20, 35));
    window.draw(speedBg);

    sf::RectangleShape speedBar({speedFill, 8.f});
    speedBar.setPosition({718.f, 36.f});
    speedBar.setFillColor(sf::Color(0, 255, 180));
    window.draw(speedBar);

    drawHearts();

    sf::Text hint(font, "W/UP jump   S/DOWN slide   A/D or arrows lane   P pause", 12);
    hint.setFillColor(sf::Color(40, 40, 60));
    hint.setPosition({160.f, 50.f});
    window.draw(hint);
}

void GameScreen::drawHearts() {
    for (int i = 0; i < 5; i++) {
        sf::RectangleShape h({16.f, 16.f});
        h.setPosition({590.f + i * 22.f, 20.f});
        h.setFillColor(i < player.hearts
            ? sf::Color(220, 40, 70)
            : sf::Color(35, 35, 50));
        window.draw(h);
    }
}

void GameScreen::drawPauseOverlay() {
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    sf::Text pt(font, "PAUSED", 52);
    pt.setFillColor(sf::Color(0, 255, 200));
    pt.setPosition({400.f - pt.getLocalBounds().size.x/2.f, 220.f});
    window.draw(pt);

    sf::Text rt(font, "P to resume    ESC for menu", 22);
    rt.setFillColor(sf::Color(120, 120, 140));
    rt.setPosition({400.f - rt.getLocalBounds().size.x/2.f, 300.f});
    window.draw(rt);
}