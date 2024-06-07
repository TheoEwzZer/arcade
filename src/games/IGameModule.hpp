/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "../core/IModule.hpp"
#include "../graphicals/IDisplayModule.hpp"
#include <iostream>
#include <string>

using arcade::core::IModule;
using arcade::graphical::IDisplayModule;
using std::string;

namespace arcade::games {
    /**
     * @class IGameModule
     * @brief Interface for game modules.
     *
     * This interface defines the common methods
     * that a game module should implement.
     */
    class IGameModule : public IModule {
        public:
            virtual ~IGameModule() = default;
            virtual void init() = 0;
            virtual int handleEvent(const int key) = 0;
            virtual vector<vector<int>> getMap() const = 0;
            virtual unsigned int getScore() const = 0;
            virtual string getName() const = 0;

            virtual string getType() const = 0;
    };
}
