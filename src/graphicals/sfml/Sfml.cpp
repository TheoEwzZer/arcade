/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "Sfml.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>

using arcade::graphical::Sfml;
using enum arcade::graphical::ADisplayModule::Direction;
using sf::Color;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::filesystem::directory_iterator;
using std::format;
using std::getline;
using std::ranges::find;
using std::size_t;
using std::string;
using std::vector;

Sfml::~Sfml()
{
    destroyResources();
}

string Sfml::init()
{
    string username = "";
    const sf::VideoMode videoMode(1920, 1080);

    while (username.length() < 3) {
        cout << "Please enter your username (minimum 3 characters): ";
        getline(cin, username);
        if (username.length() < 3) {
            cerr << "Error: The username must have at least 3 characters.\n";
        }
    }
    setUsername(username);
    destroyResources();
    mWindow = new sf::RenderWindow(videoMode, "Arcade SFML");
    return username;
}

void Sfml::loadLib(vector<string> &lib) const
{
    string filename = "";

    for (const auto &entry : directory_iterator("./lib/")) {
        filename = entry.path().filename().string();
        if (filename[0] != '.') {
            lib.push_back(filename);
        }
    }
}

void Sfml::createTexts(vector<sf::Text> &texts, const vector<string> &libraries,
    const sf::Font &font, const float positionX, const float positionY) const
{
    const size_t libSize = libraries.size();
    sf::Text text;

    for (size_t i = 0; i < libSize; i++) {
        text.setFont(font);
        text.setString(libraries[i]);
        text.setCharacterSize(24);
        text.setFillColor(Color::Red);
        text.setPosition(positionX, positionY + static_cast<float>(i * 30));
        texts.push_back(text);
    }
}

void Sfml::setOutlineColor(sf::RectangleShape &box,
    const size_t selectBox, const size_t index) const
{
    const Color boxColor = (selectBox == index) ? Color::Green : Color::White;

    box.setOutlineColor(boxColor);
}

void Sfml::setStyle(vector<sf::Text>& texts, const size_t selectIndex) const
{
    sf::Text::Style style = sf::Text::Regular;
    const size_t textSize = texts.size();

    for (size_t i = 0; i < textSize; i++) {
        style = (i == selectIndex) ? sf::Text::Underlined : sf::Text::Regular;
        texts[i].setStyle(style);
    }
}

void Sfml::menu()
{
    size_t selectGame = 0;
    size_t selectGraphic = 0;
    size_t selectBox = 0;
    vector<string> gamesLibrary;
    vector<string> graphicalLibraries;
    sf::Font font;
    sf::RectangleShape gameLibBox(sf::Vector2f(300, 300));
    sf::RectangleShape graphicalLibBox(sf::Vector2f(300, 300));
    sf::RectangleShape scoreBox(sf::Vector2f(300, 300));
    sf::RectangleShape usernameBox(sf::Vector2f(300, 300));
    sf::Text scoreText;
    sf::Text usernameText;
    vector<sf::Text> gameLibTexts;
    vector<sf::Text> graphicalLibTexts;
    sf::Event event;
    std::vector<sf::Text> scoreTexts;
    const std::vector<std::string> highScores = getHighScores();
    const size_t highScoresSize = highScores.size();
    sf::Text scoreLine;

    if ((mWindow == nullptr) || (!mWindow->isOpen())) {
        return;
    }
    font.loadFromFile("assets/BebasNeue-Regular.ttf");
    loadLib(gamesLibrary);
    gameLibBox.setPosition(200, 300);
    createTexts(gameLibTexts, gamesLibrary, font, 225, 425);
    loadLib(graphicalLibraries);
    graphicalLibBox.setPosition(600, 300);
    gameLibBox.setOutlineThickness(5);
    graphicalLibBox.setOutlineThickness(5);
    createTexts(graphicalLibTexts, graphicalLibraries, font, 625, 425);
    scoreBox.setPosition(1000, 300);
    usernameBox.setPosition(1400, 300);
    scoreText.setFont(font);
    usernameText.setFont(font);
    scoreText.setString("Score: 0");
    usernameText.setString("Username: " + getUsername());
    scoreText.setCharacterSize(24);
    usernameText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    usernameText.setFillColor(Color::White);
    scoreText.setPosition(1100, 425);
    usernameText.setPosition(1450, 425);
    scoreText.setFillColor(Color::Red);
    usernameText.setFillColor(Color::Red);
    for (size_t i = 0; i < highScoresSize; i++) {
        scoreLine.setFont(font);
        scoreLine.setString(highScores[i]);
        scoreLine.setCharacterSize(24);
        scoreLine.setFillColor(Color::Red);
        scoreLine.setPosition(1100, 425 + static_cast<float>(i * 30));
        scoreTexts.push_back(scoreLine);
    }
    while (mWindow->isOpen()) {
        while (mWindow->pollEvent(event)) {
            handleEvent(event, gamesLibrary, graphicalLibraries,
                selectGame, selectGraphic, selectBox);
        }
        setOutlineColor(gameLibBox, selectBox, 0);
        setOutlineColor(graphicalLibBox, selectBox, 1);
        setStyle(gameLibTexts, selectGame);
        setStyle(graphicalLibTexts, selectGraphic);
        mWindow->clear();
        mWindow->draw(gameLibBox);
        drawTexts(gameLibTexts);
        mWindow->draw(graphicalLibBox);
        drawTexts(graphicalLibTexts);
        mWindow->draw(scoreBox);
        for (const auto& score : scoreTexts) {
            mWindow->draw(score);
        }
        mWindow->draw(usernameBox);
        mWindow->draw(usernameText);
        mWindow->display();
    }
}

void Sfml::handleEvent(
    const sf::Event &event,
    vector<string> &gamesLibrary,
    vector<string> &graphicalLibraries,
    size_t &selectGame, size_t &selectGraphic, size_t &selectBox
) {
    if ((event.type == sf::Event::Closed) && (mWindow->isOpen())) {
        mWindow->close();
        return;
    }
    if (event.type != sf::Event::KeyPressed) {
        return;
    }
    handleKeyPress(event, gamesLibrary, graphicalLibraries,
        selectGame, selectGraphic, selectBox);
}

void Sfml::handleKeyPress(
    const sf::Event &event,
    vector<string> &gamesLibrary,
    vector<string> &graphicalLibraries,
    size_t &selectGame, size_t &selectGraphic, size_t &selectBox
) {
    const string username = getUsername();

    if ((event.key.code == sf::Keyboard::Left)
        || (event.key.code == sf::Keyboard::Right)) {
        selectBox = (selectBox + 1) % 2;
        return;
    }
    if (event.key.code == sf::Keyboard::Up) {
        handleUpKeyPress(selectBox, gamesLibrary, graphicalLibraries,
            selectGame, selectGraphic);
        return;
    }
    if (event.key.code == sf::Keyboard::Down) {
        handleDownKeyPress(selectBox, gamesLibrary, graphicalLibraries,
            selectGame, selectGraphic);
        return;
    }
    if ((event.key.code == sf::Keyboard::Enter) && (username != "")) {
        setCurrentGame(gamesLibrary[selectGame]);
        setCurrentGraphical(graphicalLibraries[selectGraphic]);
        if ((getCurrentGame() != "") && (getCurrentGraphical() != "")
            && (mWindow != nullptr)) {
            mWindow->close();
        }
    }
}

void Sfml::handleUpKeyPress(
    const size_t &selectBox,
    const vector<string> &gamesLibrary,
    const vector<string> &graphicalLibraries,
    size_t &selectGame, size_t &selectGraphic
) const {
    if (selectBox == 0) {
        if (selectGame == 0) {
            selectGame = gamesLibrary.size() - 1;
        } else {
            selectGame--;
        }
    } else {
        if (selectGraphic == 0) {
            selectGraphic = graphicalLibraries.size() - 1;
        } else {
            selectGraphic--;
        }
    }
}

void Sfml::handleDownKeyPress(
    const size_t &selectBox,
    const vector<string> &gamesLibrary,
    const vector<string> &graphicalLibraries,
    size_t &selectGame, size_t &selectGraphic
) const {
    if (selectBox == 0) {
        if (selectGame == (gamesLibrary.size() - 1)) {
            selectGame = 0;
        } else {
            selectGame++;
        }
    } else {
        if (selectGraphic == (graphicalLibraries.size() - 1)) {
            selectGraphic = 0;
        } else {
            selectGraphic++;
        }
    }
}

void Sfml::drawTexts(const vector<sf::Text> &texts)
{
    for (const auto &text : texts) {
        mWindow->draw(text);
    }
}

void Sfml::clear()
{
    mWindow->clear();
}

sf::Color Sfml::getFillColor(const int mapValue,
    const size_t i, const size_t j) const
{
    // Wall
    if (mapValue == -1) {
        return sf::Color(87, 138, 52);
    } else {
        // Empty
        if (((i + j) % 2) == 0) {
            return sf::Color(170, 215, 81);
        }
        return sf::Color(162, 209, 73);
    }
}

void Sfml::loadTextures(unordered_map<Direction, sf::Texture> &textures)
{
    textures[HEAD_RIGHT].loadFromFile("assets/head_right.png");
    textures[HEAD_LEFT].loadFromFile("assets/head_left.png");
    textures[HEAD_UP].loadFromFile("assets/head_up.png");
    textures[HEAD_DOWN].loadFromFile("assets/head_down.png");
    textures[BODY_HORIZONTAL].loadFromFile("assets/body_horizontal.png");
    textures[BODY_VERTICAL].loadFromFile("assets/body_vertical.png");
    textures[TAIL_UP].loadFromFile("assets/tail_up.png");
    textures[TAIL_DOWN].loadFromFile("assets/tail_down.png");
    textures[TAIL_LEFT].loadFromFile("assets/tail_left.png");
    textures[TAIL_RIGHT].loadFromFile("assets/tail_right.png");
    textures[BODY_BOTTOML].loadFromFile("assets/body_bottoml.png");
    textures[BODY_BOTTOMR].loadFromFile("assets/body_bottomr.png");
    textures[BODY_TOPL].loadFromFile("assets/body_topl.png");
    textures[BODY_TOPR].loadFromFile("assets/body_topr.png");
}

void Sfml::drawCell(const int cellValue, sf::RectangleShape &rectangle,
    const size_t i, const size_t j)
{
    rectangle.setSize(sf::Vector2f(50.0f, 50.0f));
    rectangle.setPosition(
        static_cast<float>(j) * 50, static_cast<float>(i) * 50
    );
    rectangle.setFillColor(getFillColor(cellValue, i, j));
    mWindow->draw(rectangle);
}

void Sfml::drawApple(sf::Sprite &appleSprite, const size_t i,
    const size_t j, const vector<vector<int>> &map)
{
    if (map[i][j] == 3) {
        appleSprite.setScale(1.2f, 1.2f);
        appleSprite.setPosition(
            (static_cast<float>(j) * 50),
            (static_cast<float>(i) * 50)
        );
    } else {
        appleSprite.setPosition(
            5 + (static_cast<float>(j) * 50),
            5 + (static_cast<float>(i) * 50)
        );
    }
    mWindow->draw(appleSprite);
}

void Sfml::drawPlayerHead(sf::Sprite &playerHeadSprite,
    unordered_map<Direction, sf::Texture> &textures,
    const vector<vector<int>> &map, const size_t i, const size_t j)
{
    Direction direction = HEAD_RIGHT;
    const int leftCell = j > 0 ? map[i][j - 1] : -1;
    const int rightCell = j < 17 ? map[i][j + 1] : -1;
    const int upCell = i > 0 ? map[i - 1][j] : -1;
    const int downCell = i < 17 ? map[i + 1][j] : -1;

    if (leftCell == 1) {
        direction = HEAD_RIGHT;
    } else if (rightCell == 1) {
        direction = HEAD_LEFT;
    } else if (upCell == 1) {
        direction = HEAD_DOWN;
    } else if (downCell == 1) {
        direction = HEAD_UP;
    }
    playerHeadSprite.setTexture(textures[direction]);
    playerHeadSprite.setPosition(
        (static_cast<float>(j) * 50),
        (static_cast<float>(i) * 50)
    );
    mWindow->draw(playerHeadSprite);
}

bool Sfml::isPlayer(const int cell)
{
    return (cell == 1) || (cell == 4);
}

void Sfml::drawPlayerBody(unordered_map<Direction, sf::Texture> &textures,
    sf::Sprite &playerBodySprite, const vector<vector<int>> &map,
    const size_t i, const size_t j)
{
    Direction textureKey = BODY_VERTICAL;
    const int leftCell = j > 0 ? map[i][j - 1] : -1;
    const int rightCell = j < 17 ? map[i][j + 1] : -1;
    const int upCell = i > 0 ? map[i - 1][j] : -1;
    const int downCell = i < 17 ? map[i + 1][j] : -1;

    if ((isPlayer(leftCell)) && (isPlayer(downCell))
        && (!isPlayer(rightCell)) && (!isPlayer(upCell))) {
            textureKey = BODY_BOTTOML;
    } else if ((isPlayer(rightCell)) && (isPlayer(downCell))
        && (!isPlayer(leftCell)) && (!isPlayer(upCell))) {
            textureKey = BODY_BOTTOMR;
    } else if ((isPlayer(leftCell)) && (isPlayer(upCell))
        && (!isPlayer(rightCell)) && (!isPlayer(downCell))) {
            textureKey = BODY_TOPL;
    } else if ((isPlayer(rightCell)) && (isPlayer(upCell))
        && (!isPlayer(leftCell)) && (!isPlayer(downCell))) {
            textureKey = BODY_TOPR;
    } else if ((isPlayer(leftCell)) && (!isPlayer(rightCell))
        && (!isPlayer(upCell)) && (!isPlayer(downCell))) {
            textureKey = TAIL_RIGHT;
    } else if ((!isPlayer(leftCell)) && (isPlayer(rightCell))
        && (!isPlayer(upCell)) && (!isPlayer(downCell))) {
            textureKey = TAIL_LEFT;
    } else if ((!isPlayer(leftCell)) && (!isPlayer(rightCell))
        && (isPlayer(upCell)) && (!isPlayer(downCell))) {
            textureKey = TAIL_DOWN;
    } else if ((!isPlayer(leftCell)) && (!isPlayer(rightCell))
        && (!isPlayer(upCell)) && (isPlayer(downCell))) {
            textureKey = TAIL_UP;
    } else if ((isPlayer(leftCell)) || (isPlayer(rightCell))) {
            textureKey = BODY_HORIZONTAL;
    }
    playerBodySprite.setTexture(textures[textureKey]);
    playerBodySprite.setPosition(
        (static_cast<float>(j) * 50),
        (static_cast<float>(i) * 50)
    );
    mWindow->draw(playerBodySprite);
}

void Sfml::drawMap(const vector<vector<int>> &map)
{
    sf::RectangleShape rectangle(sf::Vector2f(50.0f, 50.0f));
    sf::Sprite appleSprite(mAppleTexture);
    sf::Sprite bonusAppleSprite(mBonusAppleTexture);
    sf::Sprite playerHeadSprite;
    sf::Sprite playerBodySprite;
    unordered_map<Direction, sf::Texture> textures;
    const size_t mapSize = map.size();
    size_t mapLineSize = 0;

    loadTextures(textures);
    for (size_t i = 0; i < mapSize; i++) {
        mapLineSize = map[i].size();
        for (size_t j = 0; j < mapLineSize; j++) {
            drawCell(0, rectangle, i, j);
            if (map[i][j] == 2) {
                drawApple(appleSprite, i, j, map);
            } else if (map[i][j] == 3) {
                drawApple(bonusAppleSprite, i, j, map);
            } else if (map[i][j] == 4) {
                drawPlayerHead(playerHeadSprite, textures, map, i, j);
            } else if (map[i][j] == 1) {
                drawPlayerBody(textures, playerBodySprite, map, i, j);
            } else if (map[i][j] == -1) {
                drawCell(map[i][j], rectangle, i, j);
            }
        }
    }
}

void Sfml::destroyResources()
{
    if (mWindow != nullptr) {
        if (mWindow->isOpen()) {
            mWindow->close();
        }
        delete mWindow;
        mWindow = nullptr;
    }
}

int Sfml::handleGameEvent()
{
    sf::Event event;
    unordered_map<sf::Keyboard::Key, int> keyMap = {
        {sf::Keyboard::Left, 1},
        {sf::Keyboard::Up, 5},
        {sf::Keyboard::Right, 3},
        {sf::Keyboard::Down, 2},
        {sf::Keyboard::Space, 32},
        {sf::Keyboard::D, 6},
        {sf::Keyboard::Q, 4},
        {sf::Keyboard::Escape, 42},
        {sf::Keyboard::R, 7}
    };

    if (mWindow->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            if (mWindow->isOpen()) {
                mWindow->close();
            }
            return 42;
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::M) {
                destroyResources();
                return 8;
            }
            if (keyMap.contains(event.key.code)) {
                return keyMap[event.key.code];
            }
            return event.key.code;
        }
    }
    return -1;
}

void Sfml::initGame()
{
    const sf::VideoMode videoMode(950, 950);

    destroyResources();
    mWindow = new sf::RenderWindow(videoMode, "Arcade SFML");
    mAppleTexture.loadFromFile("assets/apple.png");
    mBonusAppleTexture.loadFromFile("assets/Blue_apple.png");
}

void Sfml::drawScore(const unsigned int score)
{
    sf::Text scoreText;
    sf::Font font;

    font.loadFromFile("assets/BebasNeue-Regular.ttf");
    scoreText.setFont(font);
    scoreText.setString(format("Score: {}", score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    mWindow->draw(scoreText);
}

void Sfml::display()
{
    mWindow->display();
}


bool Sfml::handleMouseEvents(const sf::Event &event, sf::Vector2i &mousePos,
    const sf::Text &restartText, const sf::Text &quitText)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        mousePos = sf::Mouse::getPosition(*mWindow);
        if (restartText.getGlobalBounds().contains(
            static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)
        )) {
            return true;
        }
        if (quitText.getGlobalBounds().contains(
            static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)
        )) {
            mWindow->close();
            ::exit(0);
        }
    }
    return false;
}

int Sfml::endGame()
{
    sf::Font font;
    sf::Event event;
    sf::Vector2i mousePos = sf::Mouse::getPosition(*mWindow);

    font.loadFromFile("assets/BebasNeue-Regular.ttf");
    sf::Text gameOverText("Game Over " + getUsername(), font);
    gameOverText.setPosition(370, 200);
    sf::Text restartText("Restart", font);
    restartText.setPosition(370, 350);
    sf::Text quitText("Quit", font);
    quitText.setPosition(370, 450);
    while (mWindow->isOpen()) {
        while (mWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow->close();
                ::exit(0);
            }
            if (handleMouseEvents(event, mousePos, restartText, quitText)) {
                return 1;
            }
        }
        mWindow->clear();
        mWindow->draw(gameOverText);
        mWindow->draw(restartText);
        mWindow->draw(quitText);
        mWindow->display();
    }
    return 0;
}
