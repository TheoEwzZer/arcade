/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "IDisplayModule.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <format>
#include <algorithm>

using std::format;
using std::getline;
using std::ifstream;
using std::min;
using std::ranges::sort;
using std::string;
using std::tuple;
using std::vector;

namespace arcade::graphical {
    /**
     * @class ADisplayModule
     * @brief Abstract base class for display modules.
     *
     * This class provides an interface for display
     * modules in an arcade game system.
     * It inherits from the IDisplayModule interface
     * and defines common methods and member variables.
     */
    class ADisplayModule : public IDisplayModule {
        public:
            ~ADisplayModule() override = default;

            enum class Direction {
                HEAD_RIGHT,
                HEAD_LEFT,
                HEAD_UP,
                HEAD_DOWN,
                BODY_HORIZONTAL,
                BODY_VERTICAL,
                TAIL_UP,
                TAIL_DOWN,
                TAIL_LEFT,
                TAIL_RIGHT,
                BODY_BOTTOML,
                BODY_BOTTOMR,
                BODY_TOPL,
                BODY_TOPR,
            };

            string init() override;
            void menu() override;
            void setCurrentGame(const string &game) override;
            const string &getCurrentGame() const override;
            void setUsername(const string &username) override;
            const string &getUsername() const override;
            void setCurrentGraphical(const string &graphical) override;
            const string &getCurrentGraphical() const override;
            void clear() override;
            void drawMap(const vector<vector<int>> &map) override;
            int handleGameEvent() override;
            void initGame() override;
            void drawScore(const unsigned int score) override;
            void display() override;
            vector<string> getHighScores() const override;
            int endGame() override;
        private:
            string mCurrentGame = "";
            string mUsername = "";
            string mCurrentGraphical = "";
    };
}
