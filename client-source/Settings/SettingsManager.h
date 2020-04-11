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
        int getInt(const std::string setting_name) const;
        bool getBool(const std::string setting_name) const;
        const std::string& getString(const std::string setting_name) const;

    private:
        static const std::string SETTINGS_FILE_PATH;

        SettingsManager();
        ~SettingsManager();

        void load();

        bool loadIntSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, int default_value);
        bool loadBoolSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, bool default_value);
        bool loadStringSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, const std::string default_value);

        std::unordered_map<std::string, int> int_settings;
        std::unordered_map<std::string, bool> bool_settings;
        std::unordered_map<std::string, std::string> string_settings;

        std::unique_ptr<nlohmann::json> json;
};
