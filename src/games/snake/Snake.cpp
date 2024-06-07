/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "Snake.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using arcade::graphical::IDisplayModule;
using enum arcade::games::Snake::Direction;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::cout;
using std::endl;
using std::ifstream;
using std::make_pair;
using std::mt19937;
using std::ofstream;
using std::random_device;
using std::size_t;
using std::stringstream;
using std::uniform_int_distribution;
using std::vector;

void arcade::games::Snake::init()
{
    const size_t height = 19;
    const size_t width = 19;
    const size_t snakeX = (width / 2) - 2;
    const size_t snakeY = height / 2;
    size_t appleX = 0;
    size_t appleY = 0;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> distrib(1, width - 2);

    mDirection = RIGHT;
    mMap.resize(19, std::vector<int>(19));
    mBonusFood = make_pair(0, 0);
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            mMap[i][j] = 0;
        }
    }
    for (size_t i = 0; i < width; i++) {
        mMap[0][i] = -1;
        mMap[height - 1][i] = -1;
    }
    for (size_t i = 0; i < height; i++) {
        mMap[i][0] = -1;
        mMap[i][width - 1] = -1;
    }
    mSnake.clear();
    for (size_t i = 0; i < 4; i++) {
        mSnake.push_back(make_pair(snakeY, snakeX + i));
        mMap[mSnake[i].first][mSnake[i].second] = (i == 3) ? 4 : 1;
    }
    do {
        appleX = distrib(gen);
        appleY = distrib(gen);
    } while (mMap[appleY][appleX] != 0);
    mMap[appleY][appleX] = 2;
}

void arcade::games::Snake::updateDirection(const int key)
{
    if ((key == 5) && (mDirection != DOWN)) {
        mDirection = UP;
    } else if ((key == 2) && (mDirection != UP)) {
        mDirection = DOWN;
    } else if ((key == 1) && (mDirection != RIGHT)) {
        mDirection = LEFT;
    } else if ((key == 3) && (mDirection != LEFT)) {
        mDirection = RIGHT;
    }
}

void arcade::games::Snake::moveSnakeHead(size_t &headX, size_t &headY) const
{
    if (mDirection == UP) {
        headX--;
    } else if (mDirection == DOWN) {
        headX++;
    } else if (mDirection == LEFT) {
        headY--;
    } else if (mDirection == RIGHT) {
        headY++;
    }
}

void arcade::games::Snake::manageBonusFood()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> distrib(1, 17);

    if (mSnake.back() == mBonusFood) {
        mScore += 2;
        mBonusFood = make_pair(0, 0);
    }
    if ((mBonusFood.first == 0) && (distrib(gen) < 2)) {
        do {
            mBonusFood.first = distrib(gen);
            mBonusFood.second = distrib(gen);
        } while (mMap[mBonusFood.first][mBonusFood.second] != 0);
        mMap[mBonusFood.first][mBonusFood.second] = 3;
        mBonusFoodTime = steady_clock::now();
    }
    const auto now = steady_clock::now();
    if ((mBonusFood.first != 0)
        && (duration_cast<seconds>(now - mBonusFoodTime).count() > 10)) {
        mMap[mBonusFood.first][mBonusFood.second] = 0;
        mBonusFood = make_pair(0, 0);
    }
}

void arcade::games::Snake::manageSpeedIncrease()
{
    const auto now = steady_clock::now();
    const auto elapsedSinceSpeedIncrease = duration_cast<seconds>(
        now - mSpeedIncreaseTime
    );

    if (elapsedSinceSpeedIncrease.count() > 1) {
        if (mDelay > 50) {
            mDelay -= 4;
        }
        mSpeedIncreaseTime = now;
    }
}

void arcade::games::Snake::manageSpeedBoost(const int key)
{
    const auto now = steady_clock::now();

    if (key == ' ') {
        mSpeedBoost = true;
        mSpeedBoostEndTime = now + std::chrono::seconds(1);
    }
    if (now > mSpeedBoostEndTime) {
        mSpeedBoost = false;
    }
}

int arcade::games::Snake::handleEvent(const int key)
{
    const auto now = steady_clock::now();
    const auto elapsed = duration_cast<milliseconds>(now - mLastMoveTime);
    auto [headX, headY] = mSnake.back();
    bool ateApple = false;
    size_t appleX = 0;
    size_t appleY = 0;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> distrib(1, 17);
    const size_t mapSize = 19 * 19;

    updateDirection(key);
    moveSnakeHead(headX, headY);
    manageSpeedBoost(key);
    if (elapsed.count() < (mSpeedBoost ? (mDelay / 2) : mDelay)) {
        return 0;
    }
    mMap[mSnake.back().first][mSnake.back().second] = 1;
    manageSpeedIncrease();
    mLastMoveTime = now;
    if ((headX < 1) || (headX > 17) || (headY < 1) || (headY > 17)) {
        return 1;
    }
    if (mMap[headX][headY] == 1) {
        return 1;
    }
    ateApple = mMap[headX][headY] == 2;
    mSnake.push_back(make_pair(headX, headY));
    mMap[headX][headY] = 4;
    if (!ateApple) {
        const auto [tailX, tailY] = mSnake.front();
        mSnake.erase(mSnake.begin());
        mMap[tailX][tailY] = 0;
    } else {
        do {
            appleX = distrib(gen);
            appleY = distrib(gen);
        } while (mMap[appleY][appleX] != 0);
        mMap[appleY][appleX] = 2;
        mScore++;
    }
    manageBonusFood();
    if (mSnake.size() == mapSize) {
        cout << "[Snake] Congratulations! You have won the game!" << endl;
        return -42;
    }
    return 0;
}

vector<vector<int>> arcade::games::Snake::getMap() const
{
    return mMap;
}

unsigned int arcade::games::Snake::getScore() const
{
    return mScore;
}

string arcade::games::Snake::getName() const
{
    return "Snake";
}
