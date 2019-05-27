#include "SettingsManager.h"

#include <fstream>

const std::string SettingsManager::SETTINGS_FILE_PATH = "Settings/settings.json";

SettingsManager::SettingsManager()
{
    loadDefaultValues();
}

SettingsManager::~SettingsManager()
{
    //dtor
}

void SettingsManager::load()
{
    //As simple as that :o
    std::ifstream is(SETTINGS_FILE_PATH);
    is >> json;

    {   //Window settings
        auto window = json["window"];
        auto end = window.end();

        auto type = window.find("type");
        auto size_x = window.find("size_x");
        auto size_y = window.find("size_y");
        auto fps_limit = window.find("fps_limit");
        auto vsync_enabled = window.find("vsync_enabled");

        if (type != end)
        {
            std::string type_str = type->get<std::string>();
            if (type_str == "bordered")
                window_settings.type = sf::Style::Default;
            if (type_str == "borderless")
                window_settings.type = sf::Style::None;
            if (type_str == "fullscreen")
                window_settings.type = sf::Style::Fullscreen;
        }
        if (size_x != end)
            window_settings.size_x = size_x->get<int>();
        if (size_y != end)
            window_settings.size_y = size_y->get<int>();
        if (fps_limit != end)
            window_settings.fps_limit = fps_limit->get<int>();
        if (vsync_enabled != end)
            window_settings.vsync_enabled = vsync_enabled->get<bool>();
    }

    {   //Audio settings
        auto audio = json["audio"];
        auto end = audio.end();

        auto sound_enabled = audio.find("sound_enabled");
        auto music_enabled = audio.find("music_enabled");
        auto sound_volume = audio.find("sound_volume");
        auto music_volume = audio.find("music_volume");

        if (sound_enabled != end)
            audio_settings.sound_enabled = sound_enabled->get<int>();
        if (music_enabled != end)
            audio_settings.music_enabled = music_enabled->get<int>();
        if (sound_volume != end)
            audio_settings.sound_volume = sound_volume->get<int>();
        if (music_volume != end)
            audio_settings.music_volume = music_volume->get<int>();
    }
}

void SettingsManager::loadDefaultValues()
{
    {   //Window settings
        window_settings.type = sf::Style::Default;
        window_settings.size_x = 800;
        window_settings.size_y = 600;
        window_settings.fps_limit = 0;
        window_settings.vsync_enabled = true;
    }
    {   //Audio settings
        audio_settings.sound_enabled = true;
        audio_settings.music_enabled = true;
        audio_settings.sound_volume = 100;
        audio_settings.music_volume = 100;
    }
}

void SettingsManager::save()
{
    //Todo
}
