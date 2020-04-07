#include "SettingsManager.h"

#include <fstream>

#include "../../common-source/Utils/UsernameCheck.h"
#include "../../common-source/Utils/Log.h"

const std::string SettingsManager::SETTINGS_FILE_PATH = "Resources/Settings/settings.json";

SettingsManager::SettingsManager()
{
}

SettingsManager::~SettingsManager()
{
}

int SettingsManager::getInt(const std::string setting_name) const
{
    auto i = int_settings.find(setting_name);

    if (i == int_settings.end())
        throw new std::out_of_range("Int setting \"" + setting_name + "\" doesn't exist.");

    return i->second;
}

bool SettingsManager::getBool(const std::string setting_name) const
{
    auto i = bool_settings.find(setting_name);

    if (i == bool_settings.end())
        throw new std::out_of_range("Bool setting \"" + setting_name + "\" doesn't exist.");

    return i->second;
}

const std::string& SettingsManager::getString(const std::string setting_name) const
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
    {
        log(ERROR, "Could not load settings file!\n");
        json = "{}"_json;
    }
    else
        is >> json;

    sf::Clock clk;

    log(INFO, "Started loading settings...\n");

    loadStringSetting({}, "language", "game_language", "eng");

    loadStringSetting({ "online" }, "address", "online_server_address", "localhost");
    loadStringSetting({ "player" }, "nick_name", "online_player_name", "Anonymous");

    loadIntSetting({ "window" }, "size_x", "window_size_x", 800);
    loadIntSetting({ "window" }, "size_y", "window_size_y", 600);
    loadIntSetting({ "window" }, "fps_limit", "fps_limit", 0);
    loadBoolSetting({ "window" }, "vsync_enabled", "vsync_enabled", true);

    loadIntSetting({ "player" }, "shoes_color", "player_shoes_color", 0xFFFF0000);
    loadIntSetting({ "player" }, "shirt_color", "player_shirt_color", 0xFFFFFFFF);
    loadIntSetting({ "player" }, "pants_color", "player_pants_color", 0xFF0000FF);

    log(INFO, "Loaded all settings in {}s.\n", clk.getElapsedTime().asSeconds());
}

bool SettingsManager::loadIntSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, int default_value)
{
    auto i = int_settings.find(setting_name);
    if (i != int_settings.end())
        int_settings.erase(i);

    int value = default_value;

    nlohmann::json js = json;
    for (const std::string& p : path)
    {
        js = js[p];
        if (!js.is_structured())
        {
            log(ERROR, "Could not get json setting \"{}\" of type int at \"", setting_name);
            for (const std::string& pp : path)
                log(ERROR, "{}/", pp);
            log(ERROR, "{}\", could not get structure \"{}\'.\n", name, p);

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
        log(ERROR, "Could not get json setting \"{}\" of type int at \"", setting_name);
        for (const std::string& pp : path)
            log(ERROR, "{}/", pp);
        log(ERROR, "{}\", could not get int \"{}\'.\n", name, name);

        int_settings.emplace(std::pair<std::string, int>(setting_name, value));
        return false;
    }

    log(INFO, "Successfully loaded int setting \"{}\" with value {}.\n", setting_name, value);

    int_settings.emplace(std::pair<std::string, int>(setting_name, value));

    return true;
}

bool SettingsManager::loadBoolSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, bool default_value)
{
    auto i = bool_settings.find(setting_name);
    if (i != bool_settings.end())
        bool_settings.erase(i);

    bool value = default_value;

    nlohmann::json js = json;
    for (const std::string& p : path)
    {
        js = js[p];
        if (!js.is_structured())
        {
            log(ERROR, "Could not get json setting \"{}\" of type bool at \"", setting_name);
            for (const std::string& pp : path)
                log(ERROR, "{}/", pp);
            log(ERROR, "{}\", could not get structure \"{}\'.\n", name, p);

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
        log(ERROR, "Could not get json setting \"{}\" of type int at \"", setting_name);
        for (const std::string& pp : path)
            log(ERROR, "{}/", pp);
        log(ERROR, "{}\", could not get bool \"{}\'.\n", name, name);

        bool_settings.emplace(std::pair<std::string, bool>(setting_name, value));
        return false;
    }

    log(INFO, "Successfully loaded bool setting \"{}\" with value {}.\n", setting_name, value);

    bool_settings.emplace(std::pair<std::string, bool>(setting_name, value));

    return true;
}

bool SettingsManager::loadStringSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, const std::string default_value)
{
    auto i = string_settings.find(setting_name);
    if (i != string_settings.end())
        string_settings.erase(i);

    std::string value = default_value;

    nlohmann::json js = json;
    for (const std::string& p : path)
    {
        js = js[p];
        if (!js.is_structured())
        {
            log(ERROR, "Could not get json setting \"{}\" of type string at \"", setting_name);
            for (const std::string& pp : path)
                log(ERROR, "{}/", pp);
            log(ERROR, "{}\", could not get structure \"{}\'.\n", name, p);

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
        log(ERROR, "Could not get json setting \"{}\" of type int at \"", setting_name);
        for (const std::string& pp : path)
            log(ERROR, "{}/", pp);
        log(ERROR, "{}\", could not get string \"{}\'.\n", name, name);

        string_settings.emplace(std::pair<std::string, std::string>(setting_name, value));
        return false;
    }

    log(INFO, "Successfully loaded string setting \"{}\" with value {}.\n", setting_name, value);

    string_settings.emplace(std::pair<std::string, std::string>(setting_name, value));

    return true;
}
