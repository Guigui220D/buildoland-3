#pragma once

#include <string>

#include <SFML/Window.hpp>

#include "../Res/Json.h"

class SettingsManager
{
    public:
        static const std::string SETTINGS_FILE_PATH;

        SettingsManager();
        ~SettingsManager();

        void load();
        void loadDefaultValues();
        void save();

        struct
        {
            int type;   //Sf::style
            unsigned int size_x;
            unsigned int size_y;
            unsigned int fps_limit;
            bool vsync_enabled;
        } window_settings;

        struct
        {
            bool sound_enabled;
            bool music_enabled;
            unsigned int sound_volume;
            unsigned int music_volume;
        } audio_settings;

    private:
        nlohmann::json json;
};
