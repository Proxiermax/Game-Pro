#include "game.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;
using namespace sf;

Game::Game()
    : window(sf::VideoMode(widthWindow, heightWindow), "SFML Game"),
    player(playerTexture, window.getSize().x, window.getSize().y),
    menu(widthWindow, heightWindow), pause(widthWindow, heightWindow)
{
    window.setFramerateLimit(frameRate);
    playerHP = 50;
    score = 0;
    charge = 10;
    frameCD = 3.0f;
    lightingCD = 3.0f;
    lightingDR = 1.0f;
    currentWave = 0;
    enemiesNumb = 0;
    enemiesPlus = 0;
    startWave = 5;
    waveDelay = 3.0f;
    gameStart = false;
    gamePause = false;
    autoShoot = false;
    lightingActivated = false;
    waveClock.restart();
    lightingCooldownClock.restart();
    lightingDurationClock.restart();

    currentState = GameState::Menu;

    if (!menuTexture.loadFromFile("Textures/rayquaza.jpg")) {
        //handle error
    }
    menuBackground.setTexture(menuTexture);
    //background.setScale(1.5f, 1.5f);
    menuBackground.setOrigin(menuBackground.getGlobalBounds().width / 2,
        menuBackground.getGlobalBounds().height / 2);
    menuBackground.setScale(1.36f, 1.36f);
    menuBackground.setPosition(sf::Vector2f(widthWindow / 2, heightWindow / 2));

    if (!inputTexture.loadFromFile("Textures/forest.jpg")) {
        //handle error
    }
    inputBackground.setTexture(inputTexture);
    //background.setScale(1.5f, 1.5f);
    inputBackground.setOrigin(inputBackground.getGlobalBounds().width / 2,
        inputBackground.getGlobalBounds().height / 2);
    inputBackground.setScale(1.0f, 1.0f);
    inputBackground.setPosition(sf::Vector2f(widthWindow / 2, heightWindow / 2));

    if (!inGameTexture.loadFromFile("Textures/grass.jpg")) {
        //handle error
    }
    mapInGame.setTexture(inGameTexture);
    //background.setScale(1.5f, 1.5f);
    mapInGame.setOrigin(mapInGame.getGlobalBounds().width / 2,
        mapInGame.getGlobalBounds().height / 2);
    mapInGame.setScale(1.8f, 1.8f);
    mapInGame.setPosition(sf::Vector2f(widthWindow / 2, heightWindow / 2));

    if (!font.loadFromFile("Texts/Roboto-Bold.ttf")); {
        //handle error
    }

    if (!playerTexture.loadFromFile("Textures/Pokemon_Unit.png")) {
        //handle error
    }
    if (!bulletTexture.loadFromFile("Textures/Pokemon_Ball.png")) {
        //handle error
    }
    if (!frameTexture.loadFromFile("Textures/frame.png")) {
        //handle error
    }
    if (!lightingTexture.loadFromFile("Textures/lighting.png")) {
        //handle error
    }
    if (!enemyTexture.loadFromFile("Textures/Pokemon_Unit.png")) {
        //handle error
    }
}

void Game::HandleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (currentState == GameState::Menu) {
            switch (event.type) {
            case sf::Event::KeyReleased:
                switch (event.key.code) {
                case sf::Keyboard::Up:
                    if (currentState == GameState::Menu) menu.MoveUp();
                    break;
                case sf::Keyboard::Down:
                    if (currentState == GameState::Menu) menu.MoveDown();
                    break;
                case sf::Keyboard::W:
                    if (currentState == GameState::Menu) menu.MoveUp();
                    break;
                case sf::Keyboard::S:
                    if (currentState == GameState::Menu) menu.MoveDown();
                    break;
                case sf::Keyboard::Return:
                    if (currentState == GameState::Menu) {
                        switch (menu.getPressItem()) {
                        case 0:
                            std::cout << "Play has been pressed" << std::endl;
                            currentState = GameState::Game;
                            inputPlayerName();
                            gameStart = true;
                            waveClock.restart();
                            currentWave = 0;
                            enemiesNumb = 0;
                            enemiesPlus = 0;
                            break;
                        case 1:
                            std::cout << "LeaderBoard has been pressed" << std::endl;
                            currentState = GameState::LeaderBoard;
                            break;
                        case 2:
                            std::cout << "Exit has been pressed" << std::endl;
                            currentState = GameState::Exit;
                            break;
                        }
                    }
                    break;
                }
                break;
            case sf::Event::Closed:
                window.close();
                break;
            }
        }
        else if (currentState == GameState::LeaderBoard) {
            switch (event.type) {
            case sf::Event::KeyReleased:
                switch (event.key.code) {
                case sf::Keyboard::Return:
                case sf::Keyboard::T:
                    currentState = GameState::Menu;
                    break;
                }
                break;
            }
        }
        else if (currentState == GameState::Pause) {
            switch (event.type) {
            case sf::Event::KeyReleased:
                switch (event.key.code) {
                case sf::Keyboard::Up:
                    if (currentState == GameState::Pause) menu.MoveUp();
                    break;
                case sf::Keyboard::Down:
                    if (currentState == GameState::Pause) menu.MoveDown();
                    break;
                case sf::Keyboard::W:
                    if (currentState == GameState::Pause) menu.MoveUp();
                    break;
                case sf::Keyboard::S:
                    if (currentState == GameState::Pause) menu.MoveDown();
                    break;
                case sf::Keyboard::Return:
                    if (currentState == GameState::Pause) {
                        switch (menu.getPressItem()) {
                        case 0:
                            currentState = GameState::Game;

                            break;
                        case 1:
                            currentState = GameState::Menu;

                            break;
                        }

                    }
                    break;
                }
            //case sf::Event::Closed:
            //    window.close();
                break;
            }
        }
        else if (currentState == GameState::Game) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                currentState = GameState::Pause;

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                currentState = GameState::Menu;
                FILE* fp;
                if (fopen_s(&fp, "leaderboard.txt", "a") == 0) {
                    fprintf(fp, "%d\n", score);
                    fclose(fp);
                }
                playerHP = 50;
                score = 0;
                playerName = "";
                player.resetPosition(widthWindow, heightWindow);
                bullets.clear();
                enemies.clear();
                gameStart = false;
                waveClock.restart();
                currentWave = 0;
                enemiesNumb = 0;
                enemiesPlus = 0;
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                autoShoot = true;
            }
            else if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left) {
                autoShoot = false;
            }
        }
        else if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::update() {
    srand(time(NULL));

    //Menu--------------------------------------------------------------
    if (currentState == GameState::Menu) {
        Menu menu(window.getSize().x, window.getSize().y);
    }

    //Pause-------------------------------------------------------------
    else if (currentState == GameState::Pause) {
        Pause pause(window.getSize().x, window.getSize().y);
    }

    //LeaderBoard-------------------------------------------------------
    else if (currentState == GameState::LeaderBoard) {
        ifstream inputFile("leaderboard.txt");
        if (!inputFile.is_open())
            return;
        string line;
        int list = 1;
        int counter = 0;
        int boardPos_Y = 90;
        vector<int> scores;
        vector<string> strings;
        /* แยก String เป็น scores และ Username(strings) */
        while (getline(inputFile, line)) {
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while (ss >> token)
                tokens.push_back(token);
            scores.push_back(stod(tokens[1]));
            strings.push_back(tokens[0]);
        }
        /* Sort คะแนนใน Leaderboard */
        for (int i = 0; i < scores.size() - 1; i++) {
            for (int n = i + 1; n < scores.size(); n++) {
                if (scores[i] < scores[n]) {
                    int x = scores[i];
                    string str_x = strings[i];
                    scores[i] = scores[n];
                    strings[i] = strings[n];
                    scores[n] = x;
                    strings[n] = str_x;
                }
            }
        }
        int size = scores.size();
        if (size > 10)
            size = 10;
        for (int i = 0; i < size; i++) {
            /* ตกแต่ง Text ตรงนี้ */
            Text text;
            text.setFont(font);
            text.setFillColor(Color::White);
            text.setOutlineColor(Color::Black);
            text.setOutlineThickness(0.5);
            text.setStyle(Text::Bold);
            text.setString(strings[i] + " " + to_string(scores[i]));
            FloatRect shapeBounds = text.getLocalBounds();
            Vector2f shapeCenter(shapeBounds.left + shapeBounds.width / 2.f, shapeBounds.top + shapeBounds.height / 2.f);
            text.setOrigin(shapeCenter);
            text.setPosition(window.getSize().x / 2, boardPos_Y);
            texts.push_back(text);
            boardPos_Y += 30; // ระยะห่างระหว่างแต่ละ Text
        }
        leaderBoard.setFont(font);
        leaderBoard.setCharacterSize(40);
        leaderBoard.setString("LEADERBOARD");
        leaderBoard.setFillColor(sf::Color::Blue);
        leaderBoard.setOutlineThickness(3);
        leaderBoard.setOutlineColor(sf::Color::White);
        leaderBoard.setPosition(sf::Vector2f(20.0f, 20.0f));
    }

    //Game--------------------------------------------------------------
    else if (currentState == GameState::Game) {
        //Infomation---------------------------
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f playerPosition = player.getPlayerShape().getPosition();
        sf::FloatRect playerBounds = player.getPlayerShape().getGlobalBounds();

        nameText.setFont(font);
        nameText.setCharacterSize(20);
        nameText.setFillColor(sf::Color::White);
        nameText.setOutlineThickness(2);
        nameText.setOutlineColor(sf::Color::Black);
        nameText.setPosition(sf::Vector2f(20.0f, 20.0f));

        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        std::stringstream scoreShow;
        scoreShow << "Score : " << score;
        scoreText.setString(scoreShow.str());
        scoreText.setOutlineThickness(2);
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setPosition(sf::Vector2f(20.0f, 60.0f));

        HPText.setFont(font);
        HPText.setCharacterSize(20);
        HPText.setFillColor(sf::Color(250, 50, 250));
        std::stringstream HPShow;
        HPShow << "Health : " << playerHP;
        HPText.setString(HPShow.str());
        HPText.setOutlineThickness(2);
        HPText.setOutlineColor(sf::Color::Black);
        HPText.setPosition(sf::Vector2f(20.0f, 100.0f));

        chargeText.setFont(font);
        chargeText.setCharacterSize(20);
        chargeText.setFillColor(sf::Color::Red);
        std::stringstream chargeShow;
        chargeShow << "Frame Charge : " << charge;
        chargeText.setString(chargeShow.str());
        chargeText.setOutlineThickness(2);
        chargeText.setOutlineColor(sf::Color::Black);
        chargeText.setPosition(sf::Vector2f(20.0f, 140.0f));

        // Calculate delta time----------------
        deltaTime = clock.restart().asSeconds();

        //Player-------------------------------
        player.move(window.getSize().x, window.getSize().y, deltaTime);
        if (playerHP > 50) {
            playerHP = 50;
        }
        else if (playerHP == 0) {
            currentState = GameState::Menu;
            /* แสดงผล Leaderboard */
            ofstream outputFile("leaderboard.txt", ios::app);
            if (!outputFile.is_open())
                return;
            outputFile << playerName << " " << score << endl;
            outputFile.close();
            playerHP = 50;
            score = 0;
            playerName = "";
            player.resetPosition(widthWindow, heightWindow);
            bullets.clear();
            enemies.clear();
            gameStart = false;
            waveClock.restart();
            currentWave = 0;
            enemiesNumb = 0;
            enemiesPlus = 0;
        }
        else if (playerHP < 0) {
            playerHP = 0;
        }

        //Bullet-------------------------------
        if (autoShoot && bulletFireClock.getElapsedTime().asSeconds() >= bulletFire) {
            Bullet bullet(bulletTexture, playerPosition, sf::Vector2f(mousePosition),
                widthWindow, heightWindow);
            bullets.push_back(bullet);
            bulletFireClock.restart();
        }
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            bulletIt->move(bulletSpeed, deltaTime);
            if (bulletIt->outsideWindow(widthWindow, heightWindow)) {
                bulletIt = bullets.erase(bulletIt);
            }
            else {
                ++bulletIt;
            }
        }

        //Frame--------------------------------
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && charge > 0 && 
            frameClock.getElapsedTime().asSeconds() >= frameCD) {
            Frame frame(frameTexture, playerPosition, sf::Vector2f(mousePosition),
                widthWindow, heightWindow);
            frames.push_back(frame);
            charge -= 1;
            frameClock.restart();
        }
        for (auto frameIt = frames.begin(); frameIt != frames.end();) {
            frameIt->move(deltaTime);
            if (frameIt->outsideWindow(widthWindow, heightWindow)) {
                frameIt = frames.erase(frameIt);
            }
            else {
                sf::FloatRect frameBounds = frameIt->getFrameShape().getGlobalBounds();
                for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
                    if (enemyIt->getEnemyShape().getGlobalBounds().intersects(frameBounds)) {
                        enemyIt = enemies.erase(enemyIt);
                        break;
                    }
                    else {
                        ++enemyIt;
                    }
                }
                if (frameIt != frames.end()) {
                    ++frameIt;
                }
            }
        }

        //Lighting-----------------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
            lightingCooldownClock.getElapsedTime().asSeconds() >= lightingCD) {
            lightingActivated = true;
            lightingCooldownClock.restart();
            lightingDurationClock.restart();
            Lighting lighting(lightingTexture, playerPosition, widthWindow, heightWindow);
            lightings.push_back(lighting);
        }
        if (lightingActivated) {
            for (auto lightingIt = lightings.begin(); lightingIt != lightings.end();) {
                lightingIt->move(deltaTime);
                if (lightingDurationClock.getElapsedTime().asSeconds() >= lightingDR) {
                    lightingActivated = false;
                    lightingIt = lightings.erase(lightingIt);
                }
                else {
                    sf::FloatRect lightingBounds = lightingIt->getLightingShape().getGlobalBounds();
                    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
                        if (enemyIt->getEnemyShape().getGlobalBounds().intersects(lightingBounds)) {
                            enemyIt = enemies.erase(enemyIt);
                            break;
                        }
                        else {
                            ++enemyIt;
                        }
                    }
                    if (lightingIt != lightings.end()) {
                        ++lightingIt;
                    }
                }
            }
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            std::cout << "Space key is pressed" << std::endl;
        }

        //Enemy--------------------------------
        spawnEnemies();

        //Collision between player and enemies
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            enemyIt->move(player.getPlayerShape().getPosition(), widthWindow, heightWindow, deltaTime);
            if (enemyIt->enemyAssault(playerBounds)) {
                playerHP -= 2;
                score -= 1;
                enemyIt = enemies.erase(enemyIt);
            }
            else {
                ++enemyIt;
            }
        }

        // Check collision between bullets and enemies
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            bool bulletRemoved = false;
            sf::FloatRect bulletBounds = bulletIt->getBulletShape().getGlobalBounds();
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
                if (enemyIt->getEnemyShape().getGlobalBounds().intersects(bulletBounds)) {
                    bulletIt = bullets.erase(bulletIt);
                    sf::Vector2f enemyPosition = enemyIt->getEnemyShape().getPosition();
                    Item1 item1(bulletTexture, enemyPosition, widthWindow, heightWindow);
                    Item2 item2(bulletTexture, enemyPosition, widthWindow, heightWindow);
                    Item3 item3(bulletTexture, enemyPosition, widthWindow, heightWindow);
                    enemyIt = enemies.erase(enemyIt);
                    score += 2;
                    bulletRemoved = true;
                    int itemRandom = rand() % 10;
                    switch (itemRandom) {
                    case 0:
                        item1s.push_back(item1);
                        break;
                    case 5:
                        item2s.push_back(item2);
                        break;
                    case 9:
                        item3s.push_back(item3);
                        break;
                    }
                    break;
                }
                else {
                    ++enemyIt;
                }
            }
            if (!bulletRemoved) {
                ++bulletIt;
            }
        }

        //Item---------------------------------
        for (auto item1It = item1s.begin(); item1It != item1s.end();) {
            if (item1It->item1Picked(playerBounds)) {
                playerHP += 3;
                item1It = item1s.erase(item1It);
            }
            else {
                ++item1It;
            }
        }

        for (auto item2It = item2s.begin(); item2It != item2s.end();) {
            if (item2It->item2Picked(playerBounds)) {
                bulletSpeed += 10.0f;
                bulletPS += 2.0f;
                bulletFire = 1.0f / bulletPS;
                item2It = item2s.erase(item2It);
            }
            else {
                ++item2It;
            }
        }
        for (auto item3It = item3s.begin(); item3It != item3s.end();) {
            if (item3It->item3Picked(playerBounds)) {
                charge += 1;
                item3It = item3s.erase(item3It);
            }
            else {
                ++item3It;
            }
        }
    }
}

void Game::render() {
    window.clear();
    if (currentState == GameState::Menu) {
        window.draw(menuBackground);
        menu.draw(window);
    }
    else if (currentState == GameState::Pause) {
        window.draw(inputBackground);
        pause.draw(window);
    }
    else if (currentState == GameState::LeaderBoard) {
        window.draw(leaderBoard);
        auto it = texts.begin();
        while (it != texts.end()) {
            window.draw(*it);
            it++;
        }
        texts.clear();
        //window.draw(score1);
    }
    else if (currentState == GameState::Game) {
        window.draw(mapInGame);
        for (auto& item1 : item1s) {
            item1.draw(window);
        }
        for (auto& item2 : item2s) {
            item2.draw(window);
        }
        for (auto& item3 : item3s) {
            item3.draw(window);
        }
        for (auto& bullet : bullets) {
            bullet.draw(window);
        }
        for (auto& frame : frames) {
            frame.draw(window);
        }
        for (auto& lighting : lightings) {
            lighting.draw(window);
        }
        for (auto& enemy : enemies) {
            enemy.draw(window);
        }
        player.draw(window);
        window.draw(nameText);
        window.draw(scoreText);
        window.draw(HPText);
        window.draw(chargeText);
    }
    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        HandleEvents();
        update();
        render();
    }
}

void Game::inputPlayerName() {
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(30);
    inputText.setFillColor(sf::Color::White);
    inputText.setString("Enter your name :");
    inputText.setOutlineThickness(3);
    inputText.setOutlineColor(sf::Color::Black);
    inputText.setOrigin(inputText.getGlobalBounds().width / 2,
        inputText.getGlobalBounds().height / 2);
    inputText.setPosition(sf::Vector2f(widthWindow / 2.0f, heightWindow / 2.0f));

    std::string inputBuffer;

    bool enteringName = true;
    while (enteringName) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    inputBuffer += static_cast<char>(event.text.unicode);
                    inputText.setString("Enter your name :\n" + inputBuffer);
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                playerName = inputBuffer;
                enteringName = false;
                inputBuffer = "";
            }
        }

        window.clear();
        window.draw(inputBackground);
        window.draw(inputText);
        window.display();
    }

    nameText.setString("Name : " + playerName);

    FILE* fp;
    if (fopen_s(&fp, "score.txt", "a") == 0) {
        fprintf(fp, "%s ", playerName.c_str());
        fclose(fp);
    }
}

void Game::spawnEnemies() {
    if (gameStart == true) {
        if (waveClock.getElapsedTime().asSeconds() >= waveDelay) {
            if (currentWave < 10) {
                enemiesPlus = 1;
            }
            else if (currentWave < 20 && currentWave >= 10) {
                enemiesPlus = 2;
            }
            else if (currentWave >= 20) {
                enemiesPlus = +3;
            }
            enemiesNumb = startWave + enemiesPlus;
            startWave = enemiesNumb;
            for (int i = 0; i < enemiesNumb; ++i) {
                if (rand() % 2 == 0) {
                    spawnPosition_X = rand() % widthWindow;
                    spawnPosition_Y = rand() % 2 == 0 ? 0 :
                        heightWindow;
                }
                else {
                    spawnPosition_X = rand() % 2 == 0 ? 0 :
                        widthWindow;
                    spawnPosition_Y = rand() % heightWindow;
                }
                Enemy enemy(enemyTexture, sf::Vector2f(spawnPosition_X, spawnPosition_Y));
                enemies.push_back(enemy);
            }
            currentWave++;
            waveClock.restart();

            if (playerHP == 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                currentWave = 0;
                enemiesNumb = 0;
                enemiesPlus = 0;
            }
        }
    }
}
