#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "menu.h"
#include "pause.h"
#include "player.h"
#include "bullet.h"
#include "frame.h"
#include "lighting.h"
#include "enemy.h"
#include "item1.h"
#include "item2.h"
#include "item3.h"
using namespace std;
using namespace sf;

enum class GameState {
    Menu, LeaderBoard, Game, Pause, GameOver, Exit
};

class Game {
public:
    int widthWindow = 800;
    int heightWindow = 800;
    int frameRate = 60;
    float bulletSpeed = 3000.0f;
    float bulletPS = 10.0f;
    float bulletFire = 1.0f / bulletPS;

    Game();
    void run();
private:
    void HandleEvents();
    void update();
    void render();
    void inputPlayerName();
    void spawnEnemies();
    int playerHP;
    int score;
    int charge;
    int currentWave;
    int enemiesNumb;
    int enemiesPlus;
    int startWave;
    float deltaTime;
    float frameCD;
    float lightingCD;
    float lightingDR;
    float spawnPosition_X;
    float spawnPosition_Y;
    float waveDelay;
    bool gameStart;
    bool gamePause;
    bool autoShoot;
    bool lightingActivated;

    sf::RenderWindow window;
    Menu menu;
    Pause pause;
    GameState currentState;
    Player player;
    sf::Texture menuTexture;
    sf::Texture inputTexture;
    sf::Texture inGameTexture;
    sf::Texture playerTexture;
    sf::Texture bulletTexture;
    sf::Texture frameTexture;
    sf::Texture lightingTexture;
    sf::Texture enemyTexture;
    sf::Sprite menuBackground;
    sf::Sprite inputBackground;
    sf::Sprite mapInGame;

    sf::Font font;
    sf::Text leaderBoard;
    sf::Text nameText;
    sf::Text scoreText;
    sf::Text HPText;
    sf::Text chargeText;
    std::string playerName;
    std::vector<Text> texts;
    std::vector<Bullet> bullets;
    std::vector<Frame> frames;
    std::vector<Lighting> lightings;
    std::vector<Enemy> enemies;
    std::vector<Item1> item1s;
    std::vector<Item2> item2s;
    std::vector<Item3> item3s;
    sf::Vector2f spawnPosition;
    sf::Clock clock;
    sf::Clock bulletFireClock;
    sf::Clock frameClock;
    sf::Clock lightingCooldownClock;
    sf::Clock lightingDurationClock;
    sf::Clock waveClock;
};
