/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "Arcade.hpp"

int main(int argc, char **argv)
{
    arcade::core::Arcade arcade;
    const int newArgc = argc;
    const char *const *newArgv = argv;

    return arcade.run(newArgc, newArgv);
}
