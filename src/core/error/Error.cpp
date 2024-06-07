/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#include "Error.hpp"

arcade::core::ArgumentError::ArgumentError(const string &text) : mText(text) {}

const char *arcade::core::ArgumentError::what() const noexcept
{
    return mText.c_str();
}

arcade::core::LibraryError::LibraryError(const string &text) : mText(text) {}

const char *arcade::core::LibraryError::what() const noexcept
{
    return mText.c_str();
}
