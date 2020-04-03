#include "LanguageManager.h"

#include <fstream>
#include <iostream>

#include <SFML/System.hpp>

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
            std::clog << "Warning! (language) " << identifier << " isn't loaded." << std::endl;
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
        std::cerr << "Language file " << language << "could not be opened! Trying eng" << std::endl;

        std::ifstream is_eng("Resources/Languages/eng.json");

        if (!is_eng.is_open())
        {
            std::cerr << "Could not load language file! This is gonna be messy :/" << std::endl;
            json = "{}"_json;
            return;
        }
        else
        {
            std::clog << "Loaded eng as fallback language." << std::endl;
            is_eng >> json;
        }
    }
    else
        is >> json;

    success = true;

    sf::Clock clk;

    std::cout << "Started loading language... (language : " << language_setting << ")" << std::endl;

    for (nlohmann::json::iterator i = json.begin(); i != json.end(); i++)
    {
        try
        {
            std::string str = i.value();
            strings.emplace(std::pair<std::string, const std::string>(i.key(), sf::String::fromUtf8(str.begin(), str.end())));
            std::clog << "Added \"" << i.key() << "\" : " << str << "." << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << "Could not read language entry : " << e.what() << std::endl;
        }

    }

    std::cout << "Loaded language in " << clk.getElapsedTime().asSeconds() << "s." << std::endl;
}
