/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "AGameModule.hpp"

void arcade::games::AGameModule::init()
{
}

int arcade::games::AGameModule::handleEvent(const int key)
{
    (void)key;
    return -1;
}

vector<vector<int>> arcade::games::AGameModule::getMap() const
{
    return {};
}

unsigned int arcade::games::AGameModule::getScore() const
{
    return 0;
}

string arcade::games::AGameModule::getName() const
{
    return "";
}
