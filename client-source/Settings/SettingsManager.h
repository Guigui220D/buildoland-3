#pragma once

#include <string>

#include <SFML/Window.hpp>

#include "../Utils/Json.hpp"

//This class can load and save settings

//NOT FINISHED YET
class SettingsManager
{
    friend class TitleScreenState;
    public:
        SettingsManager();
        ~SettingsManager();
        //So sooooooo temporary, eww
        std::string server_address;
    private:
        static const std::string SETTINGS_FILE_PATH;

        void load();
        void loadDefaultValues();
        void save();

        //TEMPORARY
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



        nlohmann::json json;
};
