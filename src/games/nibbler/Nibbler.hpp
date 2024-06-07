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
     * @class Nibbler
     * @brief A game module that represents the Nibbler game.
     *
     * This class provides an implementation of the IGameModule interface
     * and represents the Nibbler game. The game is a simple snake game
     * where the player controls a snake that moves around the screen
     * eating food and growing in size. The player loses if the snake
     * collides with the walls or itself.
    */
    class Nibbler : public AGameModule {
        public:
            enum class Direction { RIGHT, LEFT, UP, DOWN };
            int handleEvent(const int key) override;
            void init() override;
            void manageBonusFood();
            void manageSpeedBoost(const int key);
            void manageSpeedIncrease();
            void moveNibblerHead(size_t &headX, size_t &headY) const;
            void updateDirection(const int key,
                const size_t &headX, const size_t &headY);
            [[nodiscard]] bool isCellBlocked(const int cell) const;
            void changeDirectionBasedOnCells(const int cell1, const int cell2,
                const Direction direction1, const Direction direction2);
            bool changeDirection(const size_t &headX, const size_t &headY);
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
            vector<pair<size_t, size_t>> mNibbler;
            size_t mFoodCount = 29;
    };
}

extern "C" [[nodiscard]] arcade::games::IGameModule *create()
{
    return new arcade::games::Nibbler();
}
