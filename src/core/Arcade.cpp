/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "Arcade.hpp"
#include "IModule.hpp"
#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <unistd.h>
#include <vector>

using arcade::core::Arcade;
using arcade::core::ArgumentError;
using arcade::core::LibraryError;
using arcade::games::IGameModule;
using arcade::graphical::IDisplayModule;
using std::cerr;
using std::endl;
using std::filesystem::directory_iterator;
using std::filesystem::path;
using std::format;
using std::ifstream;
using std::mt19937;
using std::ofstream;
using std::random_device;
using std::ranges::find;
using std::size_t;
using std::string;
using std::stringstream;
using std::uniform_int_distribution;
using std::unique_ptr;
using std::vector;

Arcade::~Arcade()
{
    if (mGraphicalModule != nullptr) {
        delete mGraphicalModule;
    }
    if (mGameModule != nullptr) {
        delete mGameModule;
    }
}

string Arcade::getFileName(const string &path) const
{
    return std::filesystem::path(path).filename().string();
}

template <typename TModule, typename TLoader>
void Arcade::loadNewModule(const string &currentModuleName,
    const string &moduleType,
    TModule* &currentModule)
{
    vector<string> libFiles;
    string filename;
    string nextModulePath;
    const string currentModuleNameWithoutLib = currentModuleName.substr(4);
    int result = -1;
    random_device rd;
    mt19937 gen(rd());
    size_t randomIndex = 0;
    IModule *tempModule = nullptr;
    IDisplayModule *displayModule = nullptr;

    for (const auto &entry : directory_iterator("./lib/")) {
        filename = entry.path().filename().string();
        if (filename.find(".so") == string::npos) {
            continue;
        }
        TLoader tempLoader(("lib/" + filename).c_str());
        tempModule = tempLoader.getInstance();
        if (tempModule != nullptr) {
            if ((tempModule->getType() == moduleType)
            && (filename != currentModuleNameWithoutLib)) {
                libFiles.push_back(filename);
            }
            delete tempModule;
        }
    }
    if (libFiles.empty()) {
        throw LibraryError("No other libraries found.");
    }
    uniform_int_distribution<size_t> distrib(0, libFiles.size() - 1);
    randomIndex = distrib(gen);
    nextModulePath = "lib/" + libFiles[randomIndex];
    delete currentModule;
    currentModule = nullptr;
    TLoader moduleLoader(nextModulePath.c_str());
    currentModule = moduleLoader.getInstance();
    if (currentModule == nullptr) {
        throw LibraryError("Failed to load new library.");
    }
    if (currentModule->getType() == "game") {
        auto *const gameModule = dynamic_cast<IGameModule*>(currentModule);
        if (gameModule != nullptr) {
            gameModule->init();
            mGameModule = gameModule;
        } else {
            throw LibraryError("Failed to cast to IGameModule.");
        }
    } else {
        displayModule = dynamic_cast<IDisplayModule*>(currentModule);
        if (displayModule != nullptr) {
            mGraphicalModule = displayModule;
        } else {
            throw LibraryError("Failed to cast to IDisplayModule.");
        }
    }
    result = launch(mGraphicalModule, mGameModule);
    if (result == 0) {
        loadNewModule<IGameModule, DLLoader<IGameModule>>(
            nextModulePath, "game", mGameModule
        );
        return;
    }
    if (result == 1) {
        loadNewModule<IDisplayModule, DLLoader<IDisplayModule>>(
            nextModulePath, "graphical", mGraphicalModule
        );
    }
}
void Arcade::runFromMenu(string &currentGraphicName,
    string &currentGraphicFileName, DLLoader<IDisplayModule> &graphicalLoader)
{
    string currentGameName = "";
    string newCurrentGraphicName = "";
    int result = -1;

    mUsername = mGraphicalModule->init();
    mGraphicalModule->menu();
    if ((mGraphicalModule->getCurrentGame() == "null")
        || (mGraphicalModule->getCurrentGame() == "")) {
        return;
    }
    currentGameName = "lib/" + mGraphicalModule->getCurrentGame();
    newCurrentGraphicName = "lib/" + mGraphicalModule->getCurrentGraphical();
    if (mGraphicalModule->getCurrentGraphical() != currentGraphicFileName) {
        currentGraphicName = newCurrentGraphicName;
        delete mGraphicalModule;
        mGraphicalModule = nullptr;
        graphicalLoader = DLLoader<IDisplayModule>(
            currentGraphicName.c_str()
        );
        mGraphicalModule = graphicalLoader.getInstance();
    }
    DLLoader<IGameModule> gameLoader(
        currentGameName.c_str()
    );
    delete mGameModule;
    mGameModule = nullptr;
    mGameModule = gameLoader.getInstance();
    if (mGameModule == nullptr) {
        throw LibraryError("Invalid game library.");
    }
    mGameModule->init();
    result = launch(mGraphicalModule, mGameModule);
    if (result == 0) {
        loadNewModule<IGameModule, DLLoader<IGameModule>>(
            currentGameName, "game", mGameModule
        );
        return;
    }
    if (result == 1) {
        loadNewModule<IDisplayModule, DLLoader<IDisplayModule>>(
            newCurrentGraphicName, "graphical", mGraphicalModule
        );
        return;
    }
    if (result == 2) {
        mGraphicalModule->setCurrentGame("");
        mGraphicalModule->setCurrentGraphical("");
        runFromMenu(
            currentGraphicName, currentGraphicFileName, graphicalLoader
        );
        return;
    }
    return;
}

int Arcade::run(const int argc, const char *const *const argv)
{
    string currentGraphicName = "";
    string currentGraphicFileName = "";

    try {
        if (argc != 2) {
            throw ArgumentError(
                "Usage: " + string(argv[0]) + " <graphical_library_path>"
            );
        }
        currentGraphicName = argv[1];
        currentGraphicFileName = getFileName(currentGraphicName);
        DLLoader<IDisplayModule> graphicalLoader(
            currentGraphicName.c_str()
        );
        mGraphicalModule = graphicalLoader.getInstance();
        if (mGraphicalModule == nullptr) {
            throw LibraryError("Invalid graphical library.");
        }
        runFromMenu(
            currentGraphicName, currentGraphicFileName, graphicalLoader
        );
        return 0;
    } catch (const LibraryError &libraryError) {
        cerr << libraryError.what() << endl;
        return 84;
    } catch (const ArgumentError &argumentError) {
        cerr << argumentError.what() << endl;
        return 84;
    }
}

void Arcade::updateScoreInFile(const string &gameName,
    const unsigned int score) const
{
    string line;
    ifstream fileRead("Score.txt");
    const string prefix = mUsername + " - " + gameName + ": ";
    unsigned int oldScore = 0;
    bool isGameFound = false;
    vector<string> lines;

    while (getline(fileRead, line)) {
        if (line.find(prefix) != string::npos) {
            stringstream ss(line.substr(prefix.length()));
            ss >> oldScore;
            isGameFound = true;
            if (score > oldScore) {
                line = format("{}{}", prefix, score);
            }
        }
        lines.push_back(line);
    }
    fileRead.close();
    if (!isGameFound) {
        lines.push_back(format("{}{}", prefix, score));
    }
    ofstream fileWrite("Score.txt");
    for (const string& newLine : lines) {
        fileWrite << newLine << endl;
    }
    fileWrite.close();
}

int Arcade::launch(IDisplayModule *const displayModule,
    IGameModule *const gameModule)
{
    const string gameName = gameModule->getName();
    int key = 0;
    int restart = 0;
    int status = 0;
    unsigned int score = 0;
    vector<vector<int>> map;

    displayModule->initGame();
    while (true) {
        map = gameModule->getMap();
        score = gameModule->getScore();
        displayModule->clear();
        displayModule->drawMap(map);
        displayModule->drawScore(score);
        displayModule->display();
        key = displayModule->handleGameEvent();
        status = gameModule->handleEvent(key);
        if (status == 1) {
            updateScoreInFile(gameName, score);
            restart = displayModule->endGame();
            if (restart == 1) {
                gameModule->init();
            }
        }
        if (key == 6) {
            updateScoreInFile(gameName, score);
            return 1;
        }
        if (key == 4) {
            return 0;
        }
        if (key == 42) {
            updateScoreInFile(gameName, score);
            return -1;
        }
        if (key == 7) {
            gameModule->init();
        }
        if (key == 8) {
            updateScoreInFile(gameName, score);
            return 2;
        }
        if (key == -42) {
            updateScoreInFile(gameName, score);
            return 3;
        }
    }
}
