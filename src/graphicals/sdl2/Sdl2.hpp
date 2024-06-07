/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#pragma once

#include "../ADisplayModule.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

namespace arcade::graphical {
    /**
     * @class Sdl2
     * @brief Represents an SDL2 display module for the arcade game.
     *
     * The Sdl2 class is responsible for initializing the SDL2 library,
     * creating a window and renderer,
     * handling game events, rendering game elements,
     * and managing resources such as textures and fonts.
     * It inherits from the ADisplayModule abstract base class.
     */
    class Sdl2 : public ADisplayModule {
        public:
            Sdl2() = default;
            ~Sdl2() override;

            string init() override;
            void menu() override;

            void clear() override;
            void drawMap(const vector<vector<int>> &map) override;
            int handleGameEvent() override;
            void initGame() override;
            void handleEvent(const SDL_Event &event);
            void handleMousePress();
            void renderText(const string &text, const ::SDL_Rect &rect);
            void renderOption(const pair<::SDL_Rect, string> &optionPair);
            [[nodiscard]] SDL_Color getFillColor(const int mapValue,
                const size_t i, const size_t j) const;
            void drawScore(const unsigned int score) override;
            void display() override;
            void renderTextColor(const string &text,
                const ::SDL_Rect &rect, const ::SDL_Color &color);
            int endGame() override;
            void drawCell(const int cellValue, SDL_Rect &rectangle,
                const size_t i, const size_t j);
            void loadTextures(unordered_map<Direction,SDL_Texture*> &textures);
            void drawApple(SDL_Texture *const appleTexture, const size_t i,
                const size_t j, const vector<vector<int>> &map);
            void drawPlayerHead(unordered_map<Direction,SDL_Texture*> &textures,
                const vector<vector<int>> &map, const size_t i, const size_t j);
            [[nodiscard]] bool isPlayer(const int cell);
            void drawPlayerBody(unordered_map<Direction,SDL_Texture*> &textures,
                const vector<vector<int>> &map, const size_t i, const size_t j);
            int handleMouseEvents(const ::SDL_Event &event,
                const ::SDL_Rect &restartRect, const ::SDL_Rect &quitRect);
            void destroyResources();

            string getType() const override {
                return "graphical";
            }
        private:
            string mSelectedGame;
            string mSelectedLib;
            SDL_Window *mWindow = nullptr;
            SDL_Renderer *mRenderer = nullptr;
            TTF_Font *mFont = nullptr;
            SDL_Color mTextColor;
            SDL_Color mOtherColor;
            vector<pair<SDL_Rect, string>> mOptions;
            SDL_Rect mScoreRect;
            SDL_Rect mNameRect;
            string mUsername;
            SDL_Texture *nameTexture = nullptr;
            vector<SDL_Texture*> scoreTextures;
    };
}

extern "C" [[nodiscard]] arcade::graphical::IDisplayModule *create()
{
    return new arcade::graphical::Sdl2();
}
