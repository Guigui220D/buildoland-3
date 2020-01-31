#pragma once

#include <string>
#include <initializer_list>
#include <unordered_map>

#include "../Utils/Json.hpp"

#include <SFML/System.hpp>

class LanguageManager
{
    friend class Game;

    public:
        const sf::String& getString(const std::string identifier);

        void load(const std::string language);

    private:
        const std::string language_setting;

        bool success = false;

        LanguageManager();
        ~LanguageManager();

        std::unordered_map<std::string, const sf::String> strings;

        nlohmann::json json;
};
