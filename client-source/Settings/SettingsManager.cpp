#include "SettingsManager.h"

#include <fstream>

#include <SFML/System/Clock.hpp>

const std::string SettingsManager::SETTINGS_FILE_PATH = "Resources/Settings/settings.json";

SettingsManager::SettingsManager() :
    json(new nlohmann::json())
{
}

SettingsManager::~SettingsManager()
{
}

int SettingsManager::getInt(const std::string& setting_name) const
{
    auto i = int_settings.find(setting_name);

    if (i == int_settings.end())
        throw new std::out_of_range("Int setting \"" + setting_name + "\" doesn't exist.");

    return i->second;
}

bool SettingsManager::getBool(const std::string& setting_name) const
{
    auto i = bool_settings.find(setting_name);

    if (i == bool_settings.end())
        throw new std::out_of_range("Bool setting \"" + setting_name + "\" doesn't exist.");

    return i->second;
}

const std::string& SettingsManager::getString(const std::string& setting_name) const
{
    auto i = string_settings.find(setting_name);

    if (i == string_settings.end())
        throw new std::out_of_range("String setting \"" + setting_name + "\" doesn't exist.");

    return i->second;
}

void SettingsManager::load()
{
    std::ifstream is(SETTINGS_FILE_PATH);
    if (!is.is_open())
        log(ERROR, "Could not load settings file! Defaults will be loaded.\n");
    else
        is >> *json;

    sf::Clock clk;

    log(INFO, "Started loading settings...\n");

    tryLoadSetting<std::string>({}, "language", "game_language", "eng");

    tryLoadSetting<std::string>({ "online" }, "address", "online_server_address", "localhost");
    tryLoadSetting<std::string>({ "online" }, "nick_name", "online_player_name", "anonymous");

    tryLoadSetting<int>({ "window" }, "size_x", "window_size_x", 800);
    tryLoadSetting<int>({ "window" }, "size_y", "window_size_y", 600);
    tryLoadSetting<int>({ "window" }, "fps_limit", "fps_limit", 0);
    tryLoadSetting<bool>({ "window" }, "vsync_enabled", "vsync_enabled", true);

    /*
    loadIntSetting({ "player" }, "shoes_color", "player_shoes_color", 0xFFFF0000);
    loadIntSetting({ "player" }, "shirt_color", "player_shirt_color", 0xFFFFFFFF);
    loadIntSetting({ "player" }, "pants_color", "player_pants_color", 0xFF0000FF);
    */

    log(INFO, "Loaded all settings in {}s.\n", clk.getElapsedTime().asSeconds());
}
