#include "SettingsManager.h"

#include <fstream>
#include <iostream>

#include <SFML/System.hpp>

#include "../../common-source/Utils/UsernameCheck.h"

const std::string SettingsManager::SETTINGS_FILE_PATH = "Resources/Settings/settings.json";

SettingsManager::SettingsManager()
{
}

SettingsManager::~SettingsManager()
{
}

int SettingsManager::getIntSetting(const std::string setting_name) const
{
    auto i = int_settings.find(setting_name);

    if (i == int_settings.end())
        throw new std::out_of_range("Int setting \"" + setting_name + "\" doesn't exist.");

    return i->second;
}

bool SettingsManager::getBoolSetting(const std::string setting_name) const
{
    auto i = bool_settings.find(setting_name);

    if (i == bool_settings.end())
        throw new std::out_of_range("Bool setting \"" + setting_name + "\" doesn't exist.");

    return i->second;
}

const std::string& SettingsManager::getStringSetting(const std::string setting_name) const
{
    auto i = string_settings.find(setting_name);

    if (i == string_settings.end())
        throw new std::out_of_range("String setting \"" + setting_name + "\" doesn't exist.");

    return i->second;
}

void SettingsManager::load()
{
    std::ifstream is(SETTINGS_FILE_PATH);
    is >> json;

    sf::Clock clk;

    std::cout << "Started loading settings..." << std::endl;

    loadStringSetting({ "online" }, "address", "online_server_address", "localhost");
    loadStringSetting({ "player" }, "nick_name", "online_player_name", "Anonymous");

    loadIntSetting({ "window" }, "size_x", "window_size_x", 800);
    loadIntSetting({ "window" }, "size_y", "window_size_y", 600);
    loadIntSetting({ "window" }, "fps_limit", "fps_limit", 0);
    loadBoolSetting({ "window" }, "vsync_enabled", "vsync_enabled", true);

    std::cout << "Loaded all settings in " << clk.getElapsedTime().asSeconds() << "s." << std::endl;
}

bool SettingsManager::loadIntSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, int default_value)
{
    auto i = int_settings.find(name);
    if (i != int_settings.end())
        int_settings.erase(i);

    int value = default_value;

    nlohmann::json js = json;
    for (const std::string& p : path)
    {
        js = js[p];
        if (!js.is_structured())
        {
            std::cerr << "Could not get json setting \"" << setting_name << "\" of type int at \"";
            for (const std::string& pp : path)
                std::cerr << pp << '/';
            std::cerr << name << "\", could not get structure \"" << p << "\"." << std::endl;

            int_settings.emplace(std::pair<std::string, int>(setting_name, value));
            return false;
        }
    }
    try
    {
        value = js[name].get<int>();
    }
    catch (std::exception& e)
    {
        std::cerr << "Could not get json setting \"" << setting_name << "\" of type int at \"";
        for (const std::string& pp : path)
            std::cerr << pp << '/';
        std::cerr << name << "\", could not get int \"" << name << "\"." << std::endl;

        int_settings.emplace(std::pair<std::string, int>(setting_name, value));
        return false;
    }

    std::cout << "Successfully loaded int setting \"" << setting_name << "\" with value " << value << '.' << std::endl;

    int_settings.emplace(std::pair<std::string, int>(setting_name, value));

    return true;
}

bool SettingsManager::loadBoolSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, bool default_value)
{
    auto i = bool_settings.find(name);
    if (i != bool_settings.end())
        bool_settings.erase(i);

    bool value = default_value;

    nlohmann::json js = json;
    for (const std::string& p : path)
    {
        js = js[p];
        if (!js.is_structured())
        {
            std::cerr << "Could not get json setting \"" << setting_name << "\" of type bool at \"";
            for (const std::string& pp : path)
                std::cerr << pp << '/';
            std::cerr << name << "\", could not get structure \"" << p << "\"." << std::endl;

            bool_settings.emplace(std::pair<std::string, bool>(setting_name, value));
            return false;
        }
    }
    try
    {
        value = js[name].get<bool>();
    }
    catch (std::exception& e)
    {
        std::cerr << "Could not get json setting \"" << setting_name << "\" of type bool at \"";
        for (const std::string& pp : path)
            std::cerr << pp << '/';
        std::cerr << name << "\", could not get bool \"" << name << "\"." << std::endl;

        bool_settings.emplace(std::pair<std::string, bool>(setting_name, value));
        return false;
    }

    std::cout << "Successfully loaded bool setting \"" << setting_name << "\" with value " << std::boolalpha << value << '.' << std::endl;

    bool_settings.emplace(std::pair<std::string, bool>(setting_name, value));

    return true;
}

bool SettingsManager::loadStringSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, const std::string default_value)
{
    auto i = string_settings.find(name);
    if (i != string_settings.end())
        string_settings.erase(i);

    std::string value = default_value;

    nlohmann::json js = json;
    for (const std::string& p : path)
    {
        js = js[p];
        if (!js.is_structured())
        {
            std::cerr << "Could not get json setting \"" << setting_name << "\" of type string at \"";
            for (const std::string& pp : path)
                std::cerr << pp << '/';
            std::cerr << name << "\", could not get structure \"" << p << "\"." << std::endl;

            string_settings.emplace(std::pair<std::string, std::string>(setting_name, value));
            return false;
        }
    }
    try
    {
        value = js[name].get<std::string>();
    }
    catch (std::exception& e)
    {
        std::cerr << "Could not get json setting \"" << setting_name << "\" of type string at \"";
        for (const std::string& pp : path)
            std::cerr << pp << '/';
        std::cerr << name << "\", could not get string \"" << name << "\"." << std::endl;

        string_settings.emplace(std::pair<std::string, std::string>(setting_name, value));
        return false;
    }

    std::cout << "Successfully loaded string setting \"" << setting_name << "\" with value " << value << '.' << std::endl;

    string_settings.emplace(std::pair<std::string, std::string>(setting_name, value));

    return true;
}
