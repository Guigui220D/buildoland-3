#include "LanguageManager.h"

#include <fstream>

#include <SFML/System.hpp>

#include "../../common-source/Utils/Log.h"

LanguageManager::LanguageManager()
{
}

LanguageManager::~LanguageManager()
{
}

const sf::String& LanguageManager::getFmtString(const std::string& identifier)
{
    auto i = strings.find(identifier);

    if (i == strings.end())
    {
        if (success)
            log(WARN, "Warning! (language) {} isn't loaded.\n", identifier);
        strings.emplace(std::pair<std::string, const sf::String>(identifier, identifier));
        i = strings.find(identifier);
    }

    return i->second;
}

void LanguageManager::load(const std::string& language)
{
    strings.emplace(std::pair<std::string, const std::string>("", ""));

    std::ifstream is("Resources/Languages/" + language + ".json");
    if (!is.is_open())
    {
        log(ERROR, "Language file {} could not be opened! Trying eng\n", language);

        std::ifstream is_eng("Resources/Languages/eng.json");

        if (!is_eng.is_open())
        {
            log(ERROR, "Could not load language file! This is gonna be messy :/\n");
            json = "{}"_json;
            return;
        }
        else
        {
            log(INFO, "Loaded eng as fallback language.\n");
            is_eng >> json;
        }
    }
    else
        is >> json;

    success = true;

    sf::Clock clk;

    log(INFO, "Started loading language... (language : {})", language_setting);

    for (nlohmann::json::iterator i = json.begin(); i != json.end(); i++)
    {
        try
        {
            std::string str = i.value();
            strings.emplace(std::pair<std::string, const std::string>(i.key(), sf::String::fromUtf8(str.begin(), str.end())));
            log(INFO, "Added \"{}\" : {}.\n", i.key(), str);
        }
        catch (std::exception& e)
        {
            log(ERROR, "Could not read language entry : {}\n", e.what());
        }

    }

    log(INFO, "Loaded language in {}s.\n", clk.getElapsedTime().asSeconds());
}
