/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "ADisplayModule.hpp"

string arcade::graphical::ADisplayModule::init()
{
    return "Player";
}

void arcade::graphical::ADisplayModule::menu()
{
}

void arcade::graphical::ADisplayModule::setCurrentGame(const string &game)
{
    mCurrentGame = game;
}

const string &arcade::graphical::ADisplayModule::getCurrentGame() const
{
    return mCurrentGame;
}

void arcade::graphical::ADisplayModule::setUsername(const string &username)
{
    mUsername = username;
}

const string &arcade::graphical::ADisplayModule::getUsername() const
{
    return mUsername;
}

void arcade::graphical::ADisplayModule::setCurrentGraphical(
    const string &graphical)
{
    mCurrentGraphical = graphical;
}

const string &arcade::graphical::ADisplayModule::getCurrentGraphical() const
{
    return mCurrentGraphical;
}

void arcade::graphical::ADisplayModule::clear()
{
}

void arcade::graphical::ADisplayModule::drawMap(const vector<vector<int>> &map)
{
    (void)map;
}

int arcade::graphical::ADisplayModule::handleGameEvent()
{
    return 0;
}

void arcade::graphical::ADisplayModule::initGame()
{
}

void arcade::graphical::ADisplayModule::drawScore(const unsigned int score)
{
    (void)score;
}

void arcade::graphical::ADisplayModule::display()
{
}

vector<string> arcade::graphical::ADisplayModule::getHighScores() const
{
    ifstream fileRead("Score.txt");
    vector<tuple<string, string, unsigned int>> scores;
    string line = "";
    unsigned int score = 0;
    vector<string> topScores;
    size_t numScoresToDisplay = 0;

    while (getline(fileRead, line)) {
        string username = line.substr(0, line.find(" - "));
        string game = line.substr(
            line.find(" - ") + 3,
            line.find(": ") - line.find(" - ") - 3
        );
        score = static_cast<unsigned int>(
            stoi(line.substr(line.find(": ") + 2))
        );
        scores.emplace_back(game, username, score);
    }
    fileRead.close();
    sort(scores, [](const auto &a, const auto &b) {
        return get<2>(a) > get<2>(b);
    });
    numScoresToDisplay = min<size_t>(3, scores.size());
    for (size_t i = 0; i < numScoresToDisplay; i++) {
        topScores.push_back(format("{} - {}: {}",
            get<1>(scores[i]),
            get<0>(scores[i]),
            get<2>(scores[i])
        ));
    }
    return topScores;
}

int arcade::graphical::ADisplayModule::endGame()
{
    return 0;
}
