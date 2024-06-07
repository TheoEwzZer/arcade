/*
** EPITECH PROJECT, 2024
** Arcade
** File description:
** A Retro Platform
*/

#pragma once

#include <string>

using std::string;

namespace arcade::core {
    /**
     * @brief The base interface for all modules.
     *
     * This interface defines the common functionality
     * that all modules should implement.
     */
    class IModule {
        public:
            virtual ~IModule() = default;

            virtual string getType() const = 0;
    };
}
