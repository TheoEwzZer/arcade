/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#pragma once

#include <stdexcept>
#include <string>

using std::exception;
using std::string;

namespace arcade::core {
    /**
     * @class ArgumentError
     * @brief Represents an error that occurs when an argument is invalid.
    */
    class ArgumentError : public exception {
        public:
            explicit ArgumentError(const string &text);

            const char *what() const noexcept override;
        private:
            string mText;
    };

    /**
     * @class LibraryError
     * @brief Represents an error that occurs in a library.
     */
    class LibraryError : public exception {
        public:
            explicit LibraryError(const string &text);

            const char *what() const noexcept override;
        private:
            string mText;
    };
}
