#pragma once

#include <unordered_map>

#include <SFML/Audio.hpp>

class ResourceManager;

class AudioManager
{
    public:
        AudioManager(ResourceManager& resource_manager);
        ~AudioManager();

        /**
         * Plays a music from the resource manager using its name
         * Updates the sounds to remove those that have finished playing
         */
        void update();

        /**
         * Plays a music from the resource manager using its name
         * @param name : the name of the music in the resource manager
         */
        void playMusic(const std::string name);
        /**
         * Plays a sound from the resource manager using its name
         * @param name : the name of the sound in the resource manager
         * @return The id of the sound being played (0 if failed)
         */
        unsigned int playSound(const std::string name);

        /**
         * Stops a music if it's playing
         * @param name : the name of the music in the resource manager
         */
        void stopMusic(const std::string name);
        /**
         * Stops a sound if it's playing
         * @param name : the name of the sound in the resource manager
         */
        void stopSound(unsigned int id);


    private:
        ResourceManager& resource_manager;

        std::unordered_map<unsigned int, sf::Sound> playing_sounds;

        unsigned int next_sound_id = 1;
};
