/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#pragma once

#include "../ADisplayModule.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <unordered_map>

using std::vector;
using std::array;
using std::unordered_map;

namespace arcade::graphical {
    /**
     * @class Sfml
     * @brief Represents the SFML display module for the arcade game.
     *
     * The Sfml class is responsible for handling the
     * graphical display using the SFML library.
     * It inherits from the ADisplayModule class and
     * implements its pure virtual functions.
     *
     * The class provides methods for initializing the display,
     * handling events, drawing texts and shapes,
     * loading libraries, drawing the game map, handling game events, and more.
     */
    class Sfml : public ADisplayModule {
        public:
            Sfml() = default;
            ~Sfml() override;

            string init() override;
            void menu() override;
            void handleEvent(
                const sf::Event &event,
                vector<string> &gamesLibrary,
                vector<string> &graphicalLibraries,
                size_t &selectGame, size_t &selectGraphic, size_t &selectBox
            );
            void handleKeyPress(
                const sf::Event &event,
                vector<string> &gamesLibrary,
                vector<string> &graphicalLibraries,
                size_t &selectGame, size_t &selectGraphic, size_t &selectBox
            );
            void handleUpKeyPress(
                const size_t &selectBox,
                const vector<string> &gamesLibrary,
                const vector<string> &graphicalLibraries,
                size_t &selectGame, size_t &selectGraphic
            ) const;
            void handleDownKeyPress(
                const size_t &selectBox,
                const vector<string> &gamesLibrary,
                const vector<string> &graphicalLibraries,
                size_t &selectGame, size_t &selectGraphic
            ) const;
            void drawTexts(const vector<sf::Text> &texts);
            void loadLib(vector<string> &lib) const;
            void createTexts(vector<sf::Text> &texts,
                const vector<string> &libraries, const sf::Font &font,
                const float positionX, const float positionY) const;
            void setOutlineColor(sf::RectangleShape &box,
                const size_t selectBox, const size_t index) const;
            void setStyle(vector<sf::Text> &texts,
                const size_t selectIndex) const;

            void clear() override;
            void drawMap(const vector<vector<int>> &map) override;
            int handleGameEvent() override;
            void initGame() override;
            [[nodiscard]] sf::Color getFillColor(const int mapValue,
                const size_t i, const size_t j) const;
            void drawScore(const unsigned int score) override;
            void display() override;
            void loadTextures(unordered_map<Direction, sf::Texture> &textures);
            void drawCell(const int cellValue, sf::RectangleShape &rectangle,
                const size_t i, const size_t j);
            void drawApple(sf::Sprite &appleSprite, const size_t i,
                const size_t j, const vector<vector<int>> &map);
            void drawPlayerHead(sf::Sprite &playerHeadSprite,
                unordered_map<Direction, sf::Texture> &textures,
                const vector<vector<int>> &map, const size_t i, const size_t j);
            void drawPlayerBody(unordered_map<Direction, sf::Texture> &textures,
                sf::Sprite &playerBodySprite, const vector<vector<int>> &map,
                const size_t i, const size_t j);
            [[nodiscard]] bool isPlayer(const int cell);
            int endGame() override;
            bool handleMouseEvents(const sf::Event &event,
                sf::Vector2i &mousePos, const sf::Text &restartText,
                const sf::Text &quitText);
            void destroyResources();

            string getType() const override {
                return "graphical";
            }
        private:
            sf::RenderWindow *mWindow = nullptr;
            sf::Texture mAppleTexture;
            sf::Texture mBonusAppleTexture;
    };
}

extern "C" [[nodiscard]] arcade::graphical::IDisplayModule *create()
{
    return new arcade::graphical::Sfml();
}
