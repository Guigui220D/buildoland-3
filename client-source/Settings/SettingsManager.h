#pragma once

#include <string>
#include <initializer_list>
#include <unordered_map>

#include "../Utils/Json.hpp"

//This class can load and save settings
class SettingsManager
{
    friend class Game;
    public:
        int getIntSetting(const std::string setting_name) const;
        const std::string& getStringSetting(const std::string setting_name) const;

    private:
        static const std::string SETTINGS_FILE_PATH;

        SettingsManager();
        ~SettingsManager();

        void load();

        bool loadIntSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, int default_value);
        bool loadStringSetting(const std::initializer_list<const std::string> path, const std::string name, const std::string setting_name, const std::string default_value);

        std::unordered_map<std::string, int> int_settings;
        std::unordered_map<std::string, const std::string> string_settings;

        nlohmann::json json;
};
