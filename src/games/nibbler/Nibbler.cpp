/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "Nibbler.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using arcade::games::Nibbler;
using arcade::graphical::IDisplayModule;
using enum arcade::games::Nibbler::Direction;
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

void Nibbler::init()
{
    const size_t height = 19;
    const size_t width = 19;
    const size_t snakeX = (width / 2) - 2;
    const size_t snakeY = height - 2;

    mDirection = RIGHT;
    mBonusFood = make_pair(0, 0);
    mMap = {
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0,-1},
        {-1, 0,-1,-1,-1, 0,-1, 0,-1,-1,-1, 0,-1, 0,-1,-1,-1, 0,-1},
        {-1, 2,-1, 0,-1, 2,-1, 0, 0, 0, 0, 0,-1, 2,-1, 0,-1, 2,-1},
        {-1, 0,-1,-1,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1,-1,-1, 0,-1},
        {-1, 0, 0, 0, 0, 0,-1, 2,-1, 0,-1, 2,-1, 0, 0, 0, 2, 0,-1},
        {-1, 0,-1,-1,-1,-1,-1, 0,-1, 0,-1, 0,-1,-1,-1,-1,-1, 0,-1},
        {-1, 2,-1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,-1, 2,-1},
        {-1, 0,-1, 0,-1,-1,-1,-1,-1, 2,-1,-1,-1,-1,-1, 0,-1, 0,-1},
        {-1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,-1},
        {-1, 0,-1,-1,-1,-1,-1, 0,-1,-1,-1, 0,-1,-1,-1,-1,-1, 0,-1},
        {-1, 0, 0, 2, 0, 0, 0, 0,-1, 0,-1, 0, 0, 0, 0, 2, 0, 0,-1},
        {-1, 0,-1,-1,-1, 0,-1, 0,-1,-1,-1, 0,-1, 0,-1,-1,-1, 0,-1},
        {-1, 0,-1, 0,-1, 0,-1, 0, 0, 2, 0, 0,-1, 0,-1, 0,-1, 0,-1},
        {-1, 2,-1,-1,-1, 2,-1, 2,-1,-1,-1, 2,-1, 2,-1,-1,-1, 2,-1},
        {-1, 0, 0, 0, 0, 0,-1, 0,-1, 0,-1, 0,-1, 0, 0, 0, 0, 0,-1},
        {-1, 2,-1,-1,-1,-1,-1, 0,-1,-1,-1, 0,-1,-1,-1,-1,-1, 2,-1},
        {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    };
    mNibbler.clear();
    for (size_t i = 0; i < 4; i++) {
        mNibbler.push_back(make_pair(snakeY, snakeX + i));
        mMap[mNibbler[i].first][mNibbler[i].second] = (i == 3) ? 4 : 1;
    }
}

void Nibbler::updateDirection(const int key,
    const size_t &headX, const size_t &headY)
{
    const int leftCell = mMap[headX][headY - 1];
    const int rightCell = mMap[headX][headY + 1];
    const int upCell = mMap[headX - 1][headY];
    const int downCell = mMap[headX + 1][headY];

    if ((key == 5) && (mDirection != DOWN) && (upCell != -1)) {
        mDirection = UP;
    } else if ((key == 2) && (mDirection != UP) && (downCell != -1)) {
        mDirection = DOWN;
    } else if ((key == 1) && (mDirection != RIGHT) && (leftCell != -1)) {
        mDirection = LEFT;
    } else if ((key == 3) && (mDirection != LEFT) && (rightCell != -1)) {
        mDirection = RIGHT;
    }
}

void Nibbler::moveNibblerHead(size_t &headX, size_t &headY) const
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

void Nibbler::manageBonusFood()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> distrib(1, 17);

    if (mNibbler.back() == mBonusFood) {
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

void Nibbler::manageSpeedIncrease()
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

void Nibbler::manageSpeedBoost(const int key)
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

bool Nibbler::isCellBlocked(const int cell) const
{
    return cell == -1;
}

void Nibbler::changeDirectionBasedOnCells(const int cell1, const int cell2,
    const Direction direction1, const Direction direction2)
{
    if ((isCellBlocked(cell1)) && (!isCellBlocked(cell2))) {
        mDirection = direction2;
    }
    else if ((isCellBlocked(cell2)) && (!isCellBlocked(cell1))) {
        mDirection = direction1;
    }
}

bool Nibbler::changeDirection(const size_t &headX, const size_t &headY)
{
    const int leftCell = mMap[headX][headY - 1];
    const int rightCell = mMap[headX][headY + 1];
    const int upCell = mMap[headX - 1][headY];
    const int downCell = mMap[headX + 1][headY];

    if (isCellBlocked(leftCell) && (mDirection == LEFT)) {
        changeDirectionBasedOnCells(upCell, downCell, UP, DOWN);
        return true;
    }
    if (isCellBlocked(rightCell) && (mDirection == RIGHT)) {
        changeDirectionBasedOnCells(upCell, downCell, UP, DOWN);
        return true;
    }
    if (isCellBlocked(upCell) && (mDirection == UP)) {
        changeDirectionBasedOnCells(leftCell, rightCell, LEFT, RIGHT);
        return true;
    }
    if (isCellBlocked(downCell) && (mDirection == DOWN)) {
        changeDirectionBasedOnCells(leftCell, rightCell, LEFT, RIGHT);
        return true;
    }
    return false;
}

int Nibbler::handleEvent(const int key)
{
    const auto now = steady_clock::now();
    const auto elapsed = duration_cast<milliseconds>(now - mLastMoveTime);
    auto [headX, headY] = mNibbler.back();
    bool ateApple = false;

    updateDirection(key, headX, headY);
    if (changeDirection(headX, headY)) {
        return 0;
    }
    moveNibblerHead(headX, headY);
    manageSpeedBoost(key);
    if (elapsed.count() < (mSpeedBoost ? (mDelay / 2) : mDelay)) {
        return 0;
    }
    manageSpeedIncrease();
    mLastMoveTime = now;
    mMap[mNibbler.back().first][mNibbler.back().second] = 1;
    if (mMap[headX][headY] == 1) {
        return 1;
    }
    ateApple = mMap[headX][headY] == 2;
    mNibbler.push_back(make_pair(headX, headY));
    mMap[headX][headY] = 4;
    if (!ateApple) {
        const auto [tailX, tailY] = mNibbler.front();
        mNibbler.erase(mNibbler.begin());
        mMap[tailX][tailY] = 0;
    } else {
        mScore++;
        mFoodCount--;
    }
    manageBonusFood();
    if (mFoodCount == 0) {
        cout << "[Nibbler] Congratulations! You have won the game!" << endl;
        return -42;
    }
    return 0;
}

vector<vector<int>> Nibbler::getMap() const
{
    return mMap;
}

unsigned int Nibbler::getScore() const
{
    return mScore;
}

string Nibbler::getName() const
{
    return "Nibbler";
}
