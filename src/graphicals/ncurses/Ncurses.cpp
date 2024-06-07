/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "Ncurses.hpp"
#include <array>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <unordered_map>

using std::unordered_map;

arcade::graphical::Ncurses::~Ncurses()
{
    for (auto *&win : mGameBoxes) {
        if (win != nullptr) {
            ::delwin(win);
            win = nullptr;
        }
    }
    for (auto *&win : mLibraryBoxes) {
        if (win != nullptr) {
            ::delwin(win);
            win = nullptr;
        }
    }
    ::endwin();
}

string arcade::graphical::Ncurses::init()
{
    ::initscr();
    ::curs_set(0);
    ::keypad(::stdscr, TRUE);
    mSnakeBox = ::newwin(5, 30, 0, 0);
    mNibblerBox = ::newwin(5, 30, 0, 30);
    mUserFieldBox = ::newwin(5, 30, 0, 60);
    mScoresBox = ::newwin(5, 30, 0, 90);
    mSDL2Box = ::newwin(5, 30, 5, 0);
    mNcursesBox = ::newwin(5, 30, 5, 30);
    mSFMLBox = ::newwin(5, 30, 5, 60);
    ::mvwprintw(mUserFieldBox, 1, 1, "Your Username: ");
    ::wrefresh(mUserFieldBox);
    ::echo();
    wgetstr(mUserFieldBox, const_cast<char *>(username.c_str()));
    ::noecho();
    ::start_color();
    return username;
}

void arcade::graphical::Ncurses::handleKeyPress(const int ch)
{
    switch (ch) {
        case KEY_LEFT:
            decrementCurrentBox();
            break;
        case KEY_RIGHT:
            incrementCurrentBox();
            break;
        case 'q':
            decrementCurrentLibrary();
            break;
        case 'd':
            incrementCurrentLibrary();
            break;
        case 10:
            handleEnterKey();
            break;
        default:
            break;
    }
}

void arcade::graphical::Ncurses::decrementCurrentBox()
{
    mCurrentBox--;
    if (mCurrentBox < 0) {
        mCurrentBox = 1;
    }
    updateGameBoxes();
}

void arcade::graphical::Ncurses::incrementCurrentBox()
{
    mCurrentBox++;
    if (mCurrentBox > 1) {
        mCurrentBox = 0;
    }
    updateGameBoxes();
}

void arcade::graphical::Ncurses::decrementCurrentLibrary()
{
    mCurrentLibrary--;
    if (mCurrentLibrary < 0) {
        mCurrentLibrary = 2;
    }
    updateLibraryBoxes();
}

void arcade::graphical::Ncurses::incrementCurrentLibrary()
{
    mCurrentLibrary++;
    if (mCurrentLibrary > 2) {
        mCurrentLibrary = 0;
    }
    updateLibraryBoxes();
}

void arcade::graphical::Ncurses::updateGameBoxes()
{
    for (auto *const box : mGameBoxes) {
        wattron(box, COLOR_PAIR(2));
    }
    wattron(mGameBoxes[mCurrentBox], COLOR_PAIR(1));
}

void arcade::graphical::Ncurses::updateLibraryBoxes()
{
    for (auto *const box : mLibraryBoxes) {
        wattron(box, COLOR_PAIR(1));
    }
    wattron(mLibraryBoxes[mCurrentLibrary], COLOR_PAIR(2));
}

void arcade::graphical::Ncurses::handleEnterKey()
{
    if (mCurrentBox == 1) {
        setCurrentGame("arcade_snake.so");
    } else if (mCurrentBox == 0) {
        setCurrentGame("arcade_nibbler.so");
    }
    if (mCurrentLibrary == 0) {
        setCurrentGraphical("arcade_sdl2.so");
    } else if (mCurrentLibrary == 1) {
        setCurrentGraphical("arcade_ncurses.so");
    } else if (mCurrentLibrary == 2) {
        setCurrentGraphical("arcade_sfml.so");
    }
}

void arcade::graphical::Ncurses::navigateWindows()
{
    int ch = 0;

    mGameBoxes = {mSnakeBox, mNibblerBox};
    mLibraryBoxes = {mSDL2Box, mNcursesBox, mSFMLBox};
    ::init_pair(1, COLOR_WHITE, COLOR_BLACK);
    ::init_pair(2, COLOR_GREEN, COLOR_BLACK);
    ::nodelay(::stdscr, TRUE);
    ch = getch();
    if (ch == ERR) {
        return;
    }
    if (ch == 27) {
        setCurrentGame("null");
        setCurrentGraphical("null");
    }
    handleKeyPress(ch);
}

void arcade::graphical::Ncurses::menu()
{
    const vector<string> highScores = getHighScores();
    const size_t highScoresSize = highScores.size();

    while ((getCurrentGame() == "") || (getCurrentGraphical() == "")) {
        ::clear();
        ::box(mSnakeBox, 0, 0);
        ::mvwprintw(mSnakeBox, 2, 12, "Snake");
        ::wrefresh(mSnakeBox);
        ::box(mNibblerBox, 0, 0);
        ::mvwprintw(mNibblerBox, 2, 11, "Nibbler");
        ::wrefresh(mNibblerBox);
        ::init_pair(3, COLOR_BLUE, COLOR_BLACK);
        ::box(mUserFieldBox, 0, 0);
        wattron(mUserFieldBox, COLOR_PAIR(4));
        ::mvwprintw(mUserFieldBox, 0, 1, "Username:");
        wattroff(mUserFieldBox, COLOR_PAIR(4));
        ::wrefresh(mUserFieldBox);
        ::init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        ::box(mScoresBox, 0, 0);
        wattron(mScoresBox, COLOR_PAIR(3));
        ::mvwprintw(mScoresBox, 0, 1, "Scores:");
        wattroff(mScoresBox, COLOR_PAIR(3));
        for (size_t i = 0; i < highScoresSize; i++) {
            ::mvwprintw(mScoresBox, static_cast<int>(i) + 1,
                1, "%s", highScores[i].c_str());
        }
        ::wrefresh(mScoresBox);
        ::box(mSDL2Box, 0, 0);
        ::mvwprintw(mSDL2Box, 2, 12, "SDL2");
        ::wrefresh(mSDL2Box);
        ::box(mNcursesBox, 0, 0);
        ::mvwprintw(mNcursesBox, 2, 11, "Ncurses");
        ::wrefresh(mNcursesBox);
        ::box(mSFMLBox, 0, 0);
        ::mvwprintw(mSFMLBox, 2, 12, "SFML");
        ::wrefresh(mSFMLBox);
        navigateWindows();
    }
    ::clear();
    ::endwin();
}

void arcade::graphical::Ncurses::clear()
{
    ::clear();
}

void arcade::graphical::Ncurses::drawMap(const vector<vector<int>> &map)
{
    const size_t mapSize = map.size();
    size_t mapLineSize = 0;
    size_t k = 0;
    
    ::start_color();
    ::init_pair(1, COLOR_GREEN, COLOR_BLACK);
    ::init_pair(2, COLOR_RED, COLOR_BLACK);
    ::init_pair(3, COLOR_BLUE, COLOR_BLACK);

    for (size_t i = 0; i < mapSize; i++) {
        mapLineSize = map[i].size();
        k = i + 2;
        for (size_t j = 0; j < mapLineSize; j++) {
            switch (map[i][j]) {
                // Wall
                case -1:
                    ::mvprintw(static_cast<int>(k), static_cast<int>(j), "#");
                    break;
                // Snake Body
                case 1:
                    ::attron(COLOR_PAIR(1));
                    ::mvprintw(static_cast<int>(k), static_cast<int>(j), "O");
                    ::attroff(COLOR_PAIR(1));
                    break;
                // Food
                case 2:
                    ::attron(COLOR_PAIR(2));
                    ::mvprintw(static_cast<int>(k), static_cast<int>(j), "X");
                    ::attroff(COLOR_PAIR(2));
                    break;
                // Bonus Food
                case 3:
                    ::attron(COLOR_PAIR(3));
                    ::mvprintw(static_cast<int>(k), static_cast<int>(j), "X");
                    ::attroff(COLOR_PAIR(3));
                    break;
                // Snake Head
                case 4:
                    ::attron(COLOR_PAIR(1));
                    ::mvprintw(static_cast<int>(k), static_cast<int>(j), "0");
                    ::attroff(COLOR_PAIR(1));
                    break;
                // Empty
                default:
                    ::mvprintw(static_cast<int>(k), static_cast<int>(j), " ");
                    break;
            }
        }
    }
}

void arcade::graphical::Ncurses::deleteWindows()
{
    for (auto *&win : mGameBoxes) {
        if (win != nullptr) {
            ::delwin(win);
            win = nullptr;
        }
    }
    for (auto *&win : mLibraryBoxes) {
        if (win != nullptr) {
            ::delwin(win);
            win = nullptr;
        }
    }
    ::endwin();
}

int arcade::graphical::Ncurses::handleGameEvent()
{
    const int ch = ::getch();
    unordered_map<int, int> keyMap = {
        {KEY_LEFT, 1},
        {KEY_UP, 5},
        {KEY_RIGHT, 3},
        {KEY_DOWN, 2},
        {' ', 32},
        {'d', 6},
        {'q', 4},
        {27, 42},
        {'r', 7}
    };

    if (ch == 'm') {
        deleteWindows();
        ::endwin();
        return 8;
    }
    if (keyMap.contains(ch)) {
        return keyMap[ch];
    }
    return ch;
}

void arcade::graphical::Ncurses::initGame()
{
    ::initscr();
    ::nodelay(::stdscr, true);
    ::keypad(::stdscr, true);
    ::noecho();
    ::curs_set(0);
    getmaxyx(stdscr, mMaxheight, mMaxwidth);
}

void arcade::graphical::Ncurses::drawScore(const unsigned int score)
{
    ::mvprintw(0, 0, "Score: %d", score);
}

void arcade::graphical::Ncurses::display()
{
    ::refresh();
}

int arcade::graphical::Ncurses::endGame()
{
    const string gameOverText = "Game Over " + getUsername() + "!";
    vector<string> options = {"Restart", "Quit"};
    int highlight = 0;
    bool choice = false;
    int c = -1;
    const auto optionsSize = static_cast<int>(options.size());
    size_t index = 0;

    while (!choice) {
        clear();
        ::mvprintw(0, 0, "%s", gameOverText.c_str());
        for (int i = 0; i < optionsSize; i++) {
            index = static_cast<size_t>(i);
            if (i == highlight)
                attron(A_REVERSE);
            ::mvprintw(i+2, 0, "%s", options[index].c_str());
            attroff(A_REVERSE);
        }
        c = getch();
        switch (c) {
            case KEY_UP:
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight == optionsSize) {
                    highlight = optionsSize - 1;
                }
                break;
            case 10:
                choice = true;
                break;
            default:
                break;
        }
    }
    index = static_cast<size_t>(highlight);
    ::endwin();
    if (options[index] == "Restart") {
        return 1;
    }
    if (options[index] == "Quit") {
        ::exit(0);
    }
    return 0;
}
