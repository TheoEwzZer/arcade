/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#pragma once

#include "../AGameModule.hpp"
#include <array>
#include <memory>
#include <vector>
#include <chrono>

using std::array;
using std::unique_ptr;
using std::vector;
using std::pair;
using std::chrono::steady_clock;

namespace arcade::games {
    /**
     * @class Snake
     * @brief A game module that represents the Snake game.
    */
    class Snake : public AGameModule {
        public:
            enum class Direction { RIGHT, LEFT, UP, DOWN };
            int handleEvent(const int key) override;
            void init() override;
            void manageBonusFood();
            void manageSpeedBoost(const int key);
            void manageSpeedIncrease();
            void moveSnakeHead(size_t &headX, size_t &headY) const;
            void updateDirection(const int key);
            vector<vector<int>> getMap() const override;
            unsigned int getScore() const override;
            string getName() const override;

            string getType() const override {
                return "game";
            }
        private:
            vector<vector<int>> mMap;
            bool mSpeedBoost = false;
            int mDelay = 500;
            pair<size_t, size_t> mBonusFood;
            steady_clock::time_point mBonusFoodTime = steady_clock::now();
            steady_clock::time_point mLastMoveTime = steady_clock::now();
            steady_clock::time_point mSpeedBoostEndTime = steady_clock::now();
            steady_clock::time_point mSpeedIncreaseTime = steady_clock::now();
            Direction mDirection = Direction::RIGHT;
            unsigned int mScore = 0;
            vector<pair<size_t, size_t>> mSnake;
    };
}

extern "C" [[nodiscard]] arcade::games::IGameModule *create()
{
    return new arcade::games::Snake();
}
