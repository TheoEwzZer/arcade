/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#pragma once

#include "../games/IGameModule.hpp"
#include "DLLoader.hpp"
#include <memory>
#include <string>
#include <fstream>

using arcade::core::DLLoader;
using arcade::games::IGameModule;
using arcade::graphical::IDisplayModule;
using std::unique_ptr;

namespace arcade::core {
    /**
     * @class Arcade
     * @brief The main class that represents the arcade application.
     *
     * The Arcade class is responsible for running the arcade application,
     * loading graphical and game modules,
     * and managing the interaction between them.
     * It provides methods for running the application, loading new
     * graphical and game modules, launching the modules,
     * updating the score, and more.
     */
    class Arcade {
        public:
            ~Arcade();

            int run(const int argc, const char *const *const argv);
            string getFileName(const string &path) const;
            void runFromMenu(string &currentGraphicName,
                string &currentGraphicFileName,
                DLLoader<IDisplayModule> &graphicalLoader);
            int launch(IDisplayModule *const displayModule,
                IGameModule *const gameModule);
            void updateScoreInFile(const string &gameName,
                const unsigned int score) const;
            template <typename TModule, typename TLoader>
            void loadNewModule(const string &currentModuleName,
                const string &moduleType,
                TModule *&currentModule);
        private:
            IDisplayModule *mGraphicalModule = nullptr;
            IGameModule *mGameModule = nullptr;
            string mUsername = "Player";
            const vector<string> mAllowedGames = {
                "arcade_snake.so",
                "arcade_nibbler.so",
                "arcade_pacman.so",
                "arcade_qix.so",
                "arcade_centipede.so",
                "arcade_solarfox.so"
            };
            const vector<string> mAllowedGraphicals = {
                "arcade_ncurses.so",
                "arcade_sdl2.so",
                "arcade_ndk++.so",
                "arcade_aalib.so",
                "arcade_libcaca.so",
                "arcade_allegro5.so",
                "arcade_xlib.so",
                "arcade_gtk+.so",
                "arcade_sfml.so",
                "arcade_irrlicht.so",
                "arcade_opengl.so",
                "arcade_vulkan.so",
                "arcade_qt5.so"
            };
    };
}
