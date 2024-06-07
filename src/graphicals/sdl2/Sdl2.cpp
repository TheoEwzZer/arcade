/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "Sdl2.hpp"
#include <format>
#include <iostream>
#include <utility>
#include <vector>

using arcade::graphical::Sdl2;
using enum arcade::graphical::ADisplayModule::Direction;
using std::array;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::equal_to;
using std::format;
using std::getline;
using std::pair;
using std::string;

Sdl2::~Sdl2()
{
    destroyResources();
}

string Sdl2::init()
{
    const vector<string> highScores = getHighScores();
    string username = "";

    while (username.length() < 3) {
        cout << "Please enter your username (minimum 3 characters): ";
        getline(cin, username);
        if (username.length() < 3) {
            cerr << "Error: The username must have at least 3 characters.\n";
        }
    }
    setUsername(username);
    mWindow = ::SDL_CreateWindow("Menu", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    mRenderer = ::SDL_CreateRenderer(mWindow, -1, 0);
    ::SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    ::SDL_RenderClear(mRenderer);
    ::TTF_Init();
    mFont = ::TTF_OpenFont("assets/BebasNeue-Regular.ttf", 24);
    mTextColor = {0, 0, 0, 255};
    mOtherColor = {255, 255, 255, 255};
    mOptions = {
        {{70, 90, 200, 50}, "arcade_snake.so"},
        {{70, 190, 200, 50}, "arcade_nibbler.so"},
        {{370, 90, 200, 50}, "arcade_sdl2.so"},
        {{370, 190, 200, 50}, "arcade_ncurses.so"},
        {{370, 290, 200, 50}, "arcade_sfml.so"},
        {{220, 390, 200, 50}, "Start"}
    };
    mScoreRect = {120, 20, 100, 50};
    mNameRect = {420, 20, 100, 50};
    ::SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    ::SDL_RenderClear(mRenderer);
    ::SDL_Surface *const nameSurface = ::TTF_RenderText_Solid(
        mFont, ("Name: " + username).c_str(), mOtherColor);
    nameTexture = ::SDL_CreateTextureFromSurface(mRenderer, nameSurface);
    ::SDL_FreeSurface(nameSurface);
    for (const auto& score : highScores) {
        ::SDL_Surface *const scoreSurface = ::TTF_RenderText_Solid(
            mFont, ("Score: " + score).c_str(), mOtherColor);
        ::SDL_Texture *const scoreTexture = ::SDL_CreateTextureFromSurface(
            mRenderer, scoreSurface);
        ::SDL_FreeSurface(scoreSurface);
        scoreTextures.push_back(scoreTexture);
    }
    return username;
}

void Sdl2::renderText(const string &text, const ::SDL_Rect &rect)
{
    ::SDL_Surface *const textSurface = ::TTF_RenderText_Solid(
        mFont, text.c_str(), mTextColor
    );
    ::SDL_Texture *const textTexture = ::SDL_CreateTextureFromSurface(
        mRenderer, textSurface
    );

    ::SDL_FreeSurface(textSurface);
    ::SDL_RenderCopy(mRenderer, textTexture, nullptr, &rect);
    ::SDL_DestroyTexture(textTexture);
}

void Sdl2::renderOption(const pair<::SDL_Rect, string> &optionPair)
{
    const auto& [rect, option] = optionPair;

    if ((option == mSelectedGame) || (option == mSelectedLib)) {
        ::SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
    } else if ((option == "Start")
        && (!mSelectedGame.empty()) && (!mSelectedLib.empty())) {
        ::SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
    } else {
        ::SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    }
    ::SDL_RenderFillRect(mRenderer, &rect);
    renderText(option, rect);
}

void Sdl2::menu()
{
    ::SDL_Event event = {0};
    const size_t highScoresSize = scoreTextures.size();

    while ((getCurrentGame() == "") || (getCurrentGraphical() == "")) {
        ::SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        ::SDL_RenderCopy(mRenderer, nameTexture, nullptr, &mNameRect);
        for (size_t i = 0; i < highScoresSize; i++) {
            mScoreRect.x = 50 + (static_cast<int>(i) * 150);
            ::SDL_RenderCopy(mRenderer, scoreTextures[i], nullptr, &mScoreRect);
        }
        for (const auto& optionPair : mOptions) {
            renderOption(optionPair);
        }
        ::SDL_RenderPresent(mRenderer);
        while (::SDL_PollEvent(&event) == 1) {
            handleEvent(event);
        }
    }
    if (nameTexture != nullptr) {
        ::SDL_DestroyTexture(nameTexture);
        nameTexture = nullptr;
    }
    for (auto *const texture : scoreTextures) {
        ::SDL_DestroyTexture(texture);
    }
    destroyResources();
    ::SDL_Quit();
}

void Sdl2::handleEvent(const SDL_Event &event)
{
    if (event.type == ::SDL_QUIT) {
        setCurrentGame("null");
        setCurrentGraphical("null");
    } else if (event.type == ::SDL_MOUSEBUTTONDOWN) {
        handleMousePress();
    }
}

void Sdl2::handleMousePress()
{
    ::SDL_Point point;
    bool isPointInRect = false;
    bool isGameOption = false;
    bool isLibOption = false;
    bool isStartOption = false;

    ::SDL_GetMouseState(&point.x, &point.y);
    for (const auto& [rect, option] : mOptions) {
        isPointInRect = ::SDL_PointInRect(&point, &rect);
        if (!isPointInRect) {
            continue;
        }
        isGameOption = (option == "arcade_snake.so")
            || (option == "arcade_nibbler.so");
        isLibOption = (option == "arcade_sdl2.so")
            || (option == "arcade_ncurses.so")
            || (option == "arcade_sfml.so");
        isStartOption = (option == "Start")
            && (!mSelectedGame.empty()) && (!mSelectedLib.empty());
        if (isGameOption) {
            mSelectedGame = option;
        } else if (isLibOption) {
            mSelectedLib = option;
        }
        if (isStartOption) {
            setCurrentGame(mSelectedGame);
            setCurrentGraphical(mSelectedLib);
        }
        break;
    }
}

void Sdl2::clear()
{
    ::SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    ::SDL_RenderClear(mRenderer);
}

SDL_Color Sdl2::getFillColor(const int mapValue,
    const size_t i, const size_t j) const
{
    // Wall
    if (mapValue == -1) {
        return {87, 138, 52, 255};
    } else {
        // Empty
        if (((i + j) % 2) == 0) {
            return {170, 215, 81, 255};
        }
        return {162, 209, 73, 255};
    }
}

void Sdl2::loadTextures(unordered_map<Direction, SDL_Texture*> &textures)
{
    textures[HEAD_RIGHT] =
        ::IMG_LoadTexture(mRenderer, "assets/head_right.png");
    textures[HEAD_LEFT] =
        ::IMG_LoadTexture(mRenderer, "assets/head_left.png");
    textures[HEAD_UP] =
        ::IMG_LoadTexture(mRenderer, "assets/head_up.png");
    textures[HEAD_DOWN] =
        ::IMG_LoadTexture(mRenderer, "assets/head_down.png");
    textures[BODY_HORIZONTAL] =
        ::IMG_LoadTexture(mRenderer, "assets/body_horizontal.png");
    textures[BODY_VERTICAL] =
        ::IMG_LoadTexture(mRenderer, "assets/body_vertical.png");
    textures[TAIL_UP] =
        ::IMG_LoadTexture(mRenderer, "assets/tail_up.png");
    textures[TAIL_DOWN] =
        ::IMG_LoadTexture(mRenderer, "assets/tail_down.png");
    textures[TAIL_LEFT] =
        ::IMG_LoadTexture(mRenderer, "assets/tail_left.png");
    textures[TAIL_RIGHT] =
        ::IMG_LoadTexture(mRenderer, "assets/tail_right.png");
    textures[BODY_BOTTOML] =
        ::IMG_LoadTexture(mRenderer, "assets/body_bottoml.png");
    textures[BODY_BOTTOMR] =
        ::IMG_LoadTexture(mRenderer, "assets/body_bottomr.png");
    textures[BODY_TOPL] =
        ::IMG_LoadTexture(mRenderer, "assets/body_topl.png");
    textures[BODY_TOPR] =
        ::IMG_LoadTexture(mRenderer, "assets/body_topr.png");
}

void Sdl2::drawCell(const int cellValue, SDL_Rect &rectangle,
    const size_t i, const size_t j)
{
    const SDL_Color color = getFillColor(cellValue, i, j);

    rectangle.w = 50;
    rectangle.h = 50;
    rectangle.x = static_cast<int>(j) * 50;
    rectangle.y = static_cast<int>(i) * 50;
    ::SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
    ::SDL_RenderFillRect(mRenderer, &rectangle);
}

void Sdl2::drawApple(SDL_Texture *const appleTexture, const size_t i,
    const size_t j, const vector<vector<int>> &map)
{
    SDL_Rect dst;

    dst.x = static_cast<int>(j) * 50;
    dst.y = static_cast<int>(i) * 50;
    ::SDL_QueryTexture(appleTexture, nullptr, nullptr, &dst.w, &dst.h);
    if (map[i][j] == 3) {
        dst.w = static_cast<int>(dst.w * 1.2);
        dst.h = static_cast<int>(dst.h * 1.2);
    } else {
        dst.x += 5;
        dst.y += 5;
    }
    ::SDL_RenderCopy(mRenderer, appleTexture, nullptr, &dst);
}

void Sdl2::drawPlayerHead(unordered_map<Direction, SDL_Texture*> &textures,
    const vector<vector<int>> &map, const size_t i, const size_t j)
{
    Direction direction = HEAD_RIGHT;
    const int leftCell = j > 0 ? map[i][j - 1] : -1;
    const int rightCell = j < 17 ? map[i][j + 1] : -1;
    const int upCell = i > 0 ? map[i - 1][j] : -1;
    const int downCell = i < 17 ? map[i + 1][j] : -1;
    SDL_Rect dst;

    if (leftCell == 1) {
        direction = HEAD_RIGHT;
    } else if (rightCell == 1) {
        direction = HEAD_LEFT;
    } else if (upCell == 1) {
        direction = HEAD_DOWN;
    } else if (downCell == 1) {
        direction = HEAD_UP;
    }
    dst.x = static_cast<int>(j) * 50;
    dst.y = static_cast<int>(i) * 50;
    ::SDL_QueryTexture(
        textures[direction], nullptr, nullptr, &dst.w, &dst.h
    );
    ::SDL_RenderCopy(mRenderer, textures[direction], nullptr, &dst);
}

bool Sdl2::isPlayer(const int cell)
{
    return (cell == 1) || (cell == 4);
}

void Sdl2::drawPlayerBody(unordered_map<Direction, SDL_Texture*> &textures,
    const vector<vector<int>> &map, const size_t i, const size_t j)
{
    SDL_Rect dst;
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
    dst.x = static_cast<int>(j) * 50;
    dst.y = static_cast<int>(i) * 50;
    ::SDL_QueryTexture(
        textures[textureKey], nullptr, nullptr, &dst.w, &dst.h
    );
    ::SDL_RenderCopy(mRenderer, textures[textureKey], nullptr, &dst);
}

void Sdl2::drawMap(const vector<vector<int>> &map)
{
    SDL_Rect rectangle;
    unordered_map<Direction, SDL_Texture*> textures;
    SDL_Texture *const appleTexture = ::IMG_LoadTexture(
        mRenderer, "assets/apple.png"
    );
    SDL_Texture *const BonusAppleTexture = ::IMG_LoadTexture(
        mRenderer, "assets/Blue_apple.png"
    );
    const size_t mapSize = map.size();
    size_t mapLineSize = 0;

    loadTextures(textures);
    for (size_t i = 0; i < mapSize; i++) {
        mapLineSize = map[i].size();
        for (size_t j = 0; j < mapLineSize; j++) {
            drawCell(0, rectangle, i, j);
            if ((map[i][j] == 2)) {
                drawApple(appleTexture, i, j, map);
            } else if ((map[i][j] == 3)) {
                drawApple(BonusAppleTexture, i, j, map);
            } else if (map[i][j] == 4) {
                drawPlayerHead(textures, map, i, j);
            } else if (map[i][j] == 1) {
                drawPlayerBody(textures, map, i, j);
            } else if (map[i][j] == -1) {
                drawCell(map[i][j], rectangle, i, j);
            }
        }
    }
    ::SDL_DestroyTexture(appleTexture);
    for (const auto &[key, texture] : textures) {
        ::SDL_DestroyTexture(texture);
    }
}

void Sdl2::destroyResources()
{
    if (mRenderer != nullptr) {
        ::SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
    if (mWindow != nullptr) {
        ::SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    if (mFont != nullptr) {
        ::TTF_CloseFont(mFont);
        mFont = nullptr;
    }
}

int Sdl2::handleGameEvent()
{
    SDL_Event event = {0};
    unordered_map<int, int> keyMap = {
        {SDLK_LEFT, 1},
        {SDLK_UP, 5},
        {SDLK_RIGHT, 3},
        {SDLK_DOWN, 2},
        {SDLK_SPACE, 32},
        {SDLK_d, 6},
        {SDLK_q, 4},
        {SDLK_ESCAPE, 42},
        {SDLK_r, 7}
    };
    const equal_to<int> equal;

    if (::SDL_PollEvent(&event) == 1) {
        if (event.type == SDL_QUIT) {
            destroyResources();
            return 42;
        }
        if (event.type == SDL_KEYDOWN) {
            if (equal(event.key.keysym.sym, SDLK_m)) {
                destroyResources();
                return 8;
            }
            if (keyMap.contains(event.key.keysym.sym)) {
                return keyMap[event.key.keysym.sym];
            }
            return event.key.keysym.sym;
        }
    }
    return -1;
}

void Sdl2::initGame()
{
    ::SDL_Init(SDL_INIT_VIDEO);
    if (mWindow != nullptr) {
        ::SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    mWindow = ::SDL_CreateWindow(
        "Arcade SDL2",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        950,
        950,
        SDL_WINDOW_SHOWN
    );
    mRenderer = ::SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
}

void Sdl2::drawScore(const unsigned int score)
{
    TTF_Font *font = ::TTF_OpenFont("assets/BebasNeue-Regular.ttf", 24);
    const string scoreText = format("Score: {}", score);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *textSurface = ::TTF_RenderText_Solid(
        font, scoreText.c_str(), textColor
    );
    SDL_Rect textRect = {10, 10, 0, 0};
    SDL_Texture *textTexture = ::SDL_CreateTextureFromSurface(
        mRenderer, textSurface
    );

    ::SDL_QueryTexture(textTexture, nullptr, nullptr, &textRect.w, &textRect.h);
    ::SDL_RenderCopy(mRenderer, textTexture, nullptr, &textRect);
    ::SDL_FreeSurface(textSurface);
    ::SDL_DestroyTexture(textTexture);
    ::TTF_CloseFont(font);
}

void Sdl2::display()
{
    ::SDL_RenderPresent(mRenderer);
}

void Sdl2::renderTextColor(const string &text,
    const ::SDL_Rect &rect, const ::SDL_Color &color)
{
    ::TTF_Init();
    mFont = ::TTF_OpenFont("assets/BebasNeue-Regular.ttf", 24);
    ::SDL_Surface *const surface = ::TTF_RenderText_Solid(
        mFont, text.c_str(), color
    );
    ::SDL_Texture *const texture = ::SDL_CreateTextureFromSurface(
        mRenderer, surface
    );
    ::SDL_FreeSurface(surface);
    ::SDL_RenderCopy(mRenderer, texture, nullptr, &rect);
    ::SDL_DestroyTexture(texture);
}

int Sdl2::handleMouseEvents(const ::SDL_Event &event,
    const ::SDL_Rect &restartRect, const ::SDL_Rect &quitRect)
{
    int x = 0;
    int y = 0;
    ::SDL_Point mousePoint = {x, y};

    if (event.type == SDL_QUIT) {
        ::exit(0);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        ::SDL_GetMouseState(&x, &y);
        mousePoint = {x, y};
        if (::SDL_PointInRect(&mousePoint, &restartRect) == 1) {
            return 1;
        } else if (::SDL_PointInRect(&mousePoint, &quitRect) == 1) {
            ::exit(0);
        }
    }
    return 0;
}

int Sdl2::endGame()
{
    const string gameOverText = "Game Over " + getUsername();
    const ::SDL_Rect gameOverRect = {370, 200, 200, 50};
    const ::SDL_Rect restartRect = {370, 350, 200, 50};
    const ::SDL_Rect quitRect = {370, 450, 200, 50};
    const pair<::SDL_Rect, string> restartOption = {restartRect, "Restart"};
    const pair<::SDL_Rect, string> quitOption = {quitRect, "Quit"};
    const vector<pair<::SDL_Rect, string>> endOptions = {
        restartOption, quitOption
    };
    ::SDL_Event event = {0};
    int result = 0;

    while (true) {
        ::SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
        ::SDL_RenderClear(mRenderer);
        renderTextColor(gameOverText, gameOverRect, {255, 255, 255, 255});
        for (const auto& optionPair : endOptions) {
            renderOption(optionPair);
        }
        ::SDL_RenderPresent(mRenderer);
        while (::SDL_PollEvent(&event) == 1) {
            result = handleMouseEvents(event, restartRect, quitRect);
            if (result == 1) {
                return 1;
            }
        }
    }
}
