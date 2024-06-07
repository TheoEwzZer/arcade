/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#pragma once

#include <ncurses.h>
#include "../ADisplayModule.hpp"
#include <string>
#include <array>

using std::string;
using std::array;
using std::pair;

namespace arcade::graphical {
    /**
     * @class Ncurses
     * @brief Ncurses class for handling graphical
     * display using the Ncurses library.
     *
     * This class inherits from the ADisplayModule
     * class and provides methods for
     * initializing the display, handling game events,
     * drawing the map and score,
     * and cleaning up the display.
     */
    class Ncurses : public ADisplayModule {
        public:
            Ncurses() = default;
            ~Ncurses() override;

            string init() override;
            void menu() override;
            void navigateWindows();
            void handleKeyPress(const int ch);
            void decrementCurrentBox();
            void incrementCurrentBox();
            void decrementCurrentLibrary();
            void incrementCurrentLibrary();
            void updateGameBoxes();
            void updateLibraryBoxes();
            void handleEnterKey();

            void clear() override;
            void drawMap(const vector<vector<int>> &map) override;
            int handleGameEvent() override;
            void initGame() override;
            void drawScore(const unsigned int score) override;
            void display() override;
            int endGame() override;
            void deleteWindows();

            string getType() const override {
                return "graphical";
            }
        private:
            WINDOW *mSnakeBox;
            WINDOW *mNibblerBox;
            WINDOW *mScoresBox;
            WINDOW *mUserFieldBox;
            WINDOW *mSDL2Box;
            WINDOW *mNcursesBox;
            WINDOW *mSFMLBox;
            string username;
            int mCurrentBox = 0;
            int mCurrentLibrary = 0;
            array<WINDOW*, 2> mGameBoxes;
            array<WINDOW*, 3> mLibraryBoxes;
            int mMaxheight;
            int mMaxwidth;
    };
}

extern "C" [[nodiscard]] arcade::graphical::IDisplayModule *create()
{
    return new arcade::graphical::Ncurses();
}
