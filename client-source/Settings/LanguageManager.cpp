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

const std::string& LanguageManager::getString(const std::string identifier)
{
    auto i = strings.find(identifier);

    if (i == strings.end())
    {
        strings.emplace(std::pair<std::string, const std::string>(identifier, identifier));
        i = strings.find(identifier);
    }

    return i->second;
}

void LanguageManager::load(const std::string language)
{

    std::ifstream is("Resources/Languages/" + language + ".json");
    is >> json;

    sf::Clock clk;

    std::cout << "Started loading language... (language : " << language_setting << ")" << std::endl;

    for (nlohmann::json::iterator i = json.begin(); i != json.end(); i++)
    {
        //TODO : Make all of that safe, IT'S NOT
        strings.emplace(std::pair<std::string, const std::string>(i.key(), i.value()));
        std::cout << "Added \"" << i.key() << "\" : " << i.value() << "." << std::endl;
    }

    std::cout << "Loaded language in " << clk.getElapsedTime().asSeconds() << "s." << std::endl;
}
