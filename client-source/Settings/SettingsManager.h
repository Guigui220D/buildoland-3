#pragma once

#include <string>
#include <initializer_list>
#include <unordered_map>

#include "../../external/json/JsonFwd.hpp"

//This class can load and save settings
class SettingsManager
{
    friend class GameImpl;
    friend class Game;

    public:
        int getInt(const std::string& setting_name) const;
        bool getBool(const std::string& setting_name) const;
        const std::string& getString(const std::string& setting_name) const;

    private:
        static const std::string SETTINGS_FILE_PATH;

        SettingsManager();
        ~SettingsManager();

        void load();

        template <typename T>
        bool tryLoadSetting(const std::initializer_list<const std::string> path, const std::string& name, const std::string& setting_name, T default_value);

        template<typename T>
        auto& getMap();

        std::unordered_map<std::string, int> int_settings;
        std::unordered_map<std::string, bool> bool_settings;
        std::unordered_map<std::string, std::string> string_settings;

        std::unique_ptr<nlohmann::json> json;
};

template<> inline auto& SettingsManager::getMap<int>()             { return int_settings; }
template<> inline auto& SettingsManager::getMap<bool>()            { return bool_settings; }
template<> inline auto& SettingsManager::getMap<std::string>()     { return string_settings; }

#include "../../external/json/Json.hpp"
#include "../../common-source/Utils/Log.h"

template <typename T>
bool SettingsManager::tryLoadSetting(const std::initializer_list<const std::string> path, const std::string& name, const std::string& setting_name, T default_value)
{
    auto i = getMap<T>().find(setting_name);
    if (i != getMap<T>().end())
        getMap<T>().erase(i);

    T value = default_value;

    if (!json->is_object())
    {
        getMap<T>().emplace(std::pair<std::string, T>(setting_name, value));
        return false;
    }

    nlohmann::json js = *json;
    for (const std::string& p : path)
    {
        js = js[p];
        if (!js.is_object())
        {
            log(ERROR, "Could not get json setting \"{}\", could not get structure \"{}\".\n", setting_name, p);
            getMap<T>().emplace(std::pair<std::string, T>(setting_name, value));
            return false;
        }
    }
    try
    {
        value = js[name].get<T>();
    }
    catch (std::exception& e)
    {
        log(ERROR, "Could not get json setting \"{}\", could not get value \"{}\".\n", setting_name, name);

        getMap<T>().emplace(std::pair<std::string, T>(setting_name, value));
        return false;
    }

    log(INFO, "Successfully loaded setting \"{}\" with value {}.\n", setting_name, value);

    getMap<T>().emplace(std::pair<std::string, T>(setting_name, value));

    return true;
}
