/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "IGameModule.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::format;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;

namespace arcade::games {
    /**
     * @class AGameModule
     * @brief The base class for game modules.
     *
     * This class provides an implementation of the IGameModule interface
     * and serves as a base class for specific game modules.
     */
    class AGameModule : public IGameModule {
        public:
            ~AGameModule() override = default;
            void init() override;
            int handleEvent(const int key) override;
            vector<vector<int>> getMap() const override;
            unsigned int getScore() const override;
            string getName() const override;
    };
}
