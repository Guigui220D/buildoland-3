#pragma once

#include <string>
#include <initializer_list>
#include <unordered_map>

#include "../../external/json/Json.hpp"
#include "../../external/fmt/include/fmt/format.h"

#include "../../common-source/Utils/Log.h"

#include <SFML/System.hpp>

class LanguageManager
{
    friend class Game;

    public:
        template<typename... FmtArgs>
        sf::String getString(const std::string& identifier, FmtArgs&&... args)
        {
            std::string str = (const char*)getFmtString(identifier).toUtf8().data();
            try
            {
                str = fmt::format(str, std::forward<FmtArgs>(args)...);
            }
            catch (const fmt::format_error& err)
            {
                log(ERROR, "Invalid language format for '{}' : {}\n", identifier, err.what());
            }

            sf::String sf_str;
            sf_str = sf::String::fromUtf8(str.begin(), str.end());
            return sf_str;
        }

        void load(const std::string& language);

    private:
        const sf::String& getFmtString(const std::string& identifier);

        const std::string language_setting;

        bool success = false;

        LanguageManager();
        ~LanguageManager();

        std::unordered_map<std::string, const sf::String> strings;

        nlohmann::json json;
};
