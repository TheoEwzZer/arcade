/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "../core/IModule.hpp"
#include <array>
#include <iostream>
#include <string>
#include <vector>

using arcade::core::IModule;
using std::array;
using std::endl;
using std::string;
using std::vector;

namespace arcade::graphical {
    /**
     * @brief The IDisplayModule class is an interface that defines the methods
     * required for a display module in an arcade game.
     *
     * This interface provides methods for initializing the display module,
     * handling user input, drawing the game map, managing the game state,
     * displaying the score, and retrieving high scores.
     */
    class IDisplayModule : public IModule {
        public:
            virtual ~IDisplayModule() = default;
            virtual string init() = 0;
            virtual void menu() = 0;
            virtual void setCurrentGame(const string &game) = 0;
            virtual const string &getCurrentGame() const = 0;
            virtual void setUsername(const string &username) = 0;
            virtual const string &getUsername() const = 0;
            virtual void setCurrentGraphical(const string &graphical) = 0;
            virtual const string &getCurrentGraphical() const = 0;

            virtual void clear() = 0;
            virtual void drawMap(const vector<vector<int>> &map) = 0;
            virtual int handleGameEvent() = 0;
            virtual void initGame() = 0;
            virtual void drawScore(const unsigned int score) = 0;
            virtual void display() = 0;
            virtual int endGame() = 0;
            virtual vector<string> getHighScores() const = 0;

            virtual string getType() const = 0;
    };
}
