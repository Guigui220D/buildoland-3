#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <vector>
#include <iostream>

class AssetManager
{
    public:
        AssetManager(unsigned int maxSounds = 32);
        ~AssetManager();

        static const std::string AUDIO_REPERTORY_NAME;
        static const std::string FONT_REPERTORY_NAME;
        static const std::string TEXTURE_REPERTORY_NAME;

        //Add assets
        void addMusic(sf::Music* music, const std::string name);
        void addSound(sf::SoundBuffer* sound, const std::string name);
        void addTexture(sf::Texture* texture, const std::string name);
        void addFont(sf::Font* font, const std::string name);

        //Add assets from files
        bool loadMusicFromFile(const std::string path, const std::string name);
        bool loadSoundFromFile(const std::string path, const std::string name);
        bool loadTextureFromFile(const std::string path, const std::string name);
        bool loadFontFromFile(const std::string path, const std::string name);

        //Get assets
        sf::Music* getMusic(const std::string name);
        sf::SoundBuffer* getSound(const std::string name);
        sf::Texture* getTexture(const std::string name);
        sf::Font* getFont(const std::string name);

        inline sf::Texture* getErrorTexture() { return &errorTexture; };

        //This is used to manage sounds that are playing, to delete them.
        void addPlayingSound(sf::Sound* sound);

    private:
        std::unordered_map<std::string, sf::Music*> musics;
        std::unordered_map<std::string, sf::SoundBuffer*> sounds;
        std::unordered_map<std::string, sf::Texture*> textures;
        std::unordered_map<std::string, sf::Font*> fonts;

        sf::Texture errorTexture;

        std::vector<sf::Sound*> playingSounds;
        unsigned int maxSoundsPlaying;
};
