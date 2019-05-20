#include "AudioManager.h"

#include <limits.h>

#include "ResourceManager.h"

AudioManager::AudioManager(ResourceManager& resource_manager) :
    resource_manager(resource_manager)
{
    //ctor
}

AudioManager::~AudioManager()
{
    //dtor
}

void AudioManager::update()
{
    for (auto i = playing_sounds.begin(); i != playing_sounds.end(); )
    {
        if ((*i).second.getStatus() == sf::Sound::Stopped)
        {
            playing_sounds.erase(i);
        }
        else i++;
    }
}


void AudioManager::playMusic(const std::string name)
{
    resource_manager.getMusic(name).play();
}

unsigned int AudioManager::playSound(const std::string name)
{
    auto it = playing_sounds.emplace(std::pair<unsigned int, sf::Sound>(next_sound_id, sf::Sound())).first;
    it->second.setBuffer(resource_manager.getSound(name));
    it->second.play();
    if (next_sound_id++ >= UINT_MAX)
        next_sound_id = 1;
    return it->first;   //Return the id
}


void AudioManager::stopMusic(const std::string name)
{
    resource_manager.getMusic(name).stop();
}

void AudioManager::stopSound(unsigned int id)
{
    auto it = playing_sounds.find(id);
    if (it != playing_sounds.end())
        it->second.stop();
}
