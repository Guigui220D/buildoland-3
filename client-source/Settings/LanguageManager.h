#pragma once

#include <string>
#include <initializer_list>
#include <unordered_map>

#include "../Utils/Json.hpp"

class LanguageManager
{
    friend class Game;

    public:
        const std::string& getString(const std::string identifier);

        void load(const std::string language);

    private:
        const std::string language_setting;

        LanguageManager();
        ~LanguageManager();

        std::unordered_map<std::string, const std::string> strings;

        nlohmann::json json;
};
