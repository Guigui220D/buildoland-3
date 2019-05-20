#include "ResourceManager.h"

const std::string ResourceManager::AUDIO_REPERTORY_NAME = "Res/";
const std::string ResourceManager::TEXTURE_REPERTORY_NAME = "Res/";
const std::string ResourceManager::FONT_REPERTORY_NAME = "Res/";

ResourceManager::ResourceManager(unsigned int maxSounds)
{
    maxSoundsPlaying = maxSounds;

    sf::Image errorImage;
    errorImage.create(2, 2);
    errorImage.setPixel(0, 0, sf::Color(255, 0, 255));
    errorImage.setPixel(1, 1, sf::Color(255, 0, 255));
    errorTexture.loadFromImage(errorImage);
}

ResourceManager::~ResourceManager()
{
    //Delete music
    for (auto i = musics.begin(); i != musics.end(); i++)
        delete (*i).second;
    //Delete sounds
    for (auto i = sounds.begin(); i != sounds.end(); i++)
        delete (*i).second;
    //Delete textures
    for (auto i = textures.begin(); i != textures.end(); i++)
        delete (*i).second;
    //Delete fonts
    for (auto i = fonts.begin(); i != fonts.end(); i++)
        delete (*i).second;
}

void ResourceManager::addPlayingSound(sf::Sound* sound)
{
    playingSounds.push_back(sound);
    if (playingSounds.size() > maxSoundsPlaying)
    {
        delete playingSounds.at(0);
        playingSounds.erase(playingSounds.begin());
    }
}




bool ResourceManager::addMusic(sf::Music* music, const std::string name)
{
    bool success = musics.emplace(std::pair<std::string, sf::Music*>(name, music)).second;
    if (!success)
        std::cerr << "Music \"" << name << "\" could not be added, an other music with the same name already exists." << std::endl;
    return success;
}

bool ResourceManager::addSound(sf::SoundBuffer* sound, const std::string name)
{
    bool success = sounds.emplace(std::pair<std::string, sf::SoundBuffer*>(name, sound)).second;
    if (!success)
        std::cerr << "Sound \"" << name << "\" could not be added, an other sound with the same name already exists." << std::endl;
    return success;
}

bool ResourceManager::addTexture(sf::Texture* texture, const std::string name)
{
    bool success = textures.emplace(std::pair<std::string, sf::Texture*>(name, texture)).second;
    if (!success)
        std::cerr << "Texture \"" << name << "\" could not be added, an other texture with the same name already exists." << std::endl;
    return success;
}

bool ResourceManager::addFont(sf::Font* font, const std::string name)
{
    bool success = fonts.emplace(std::pair<std::string, sf::Font*>(name, font)).second;
    if (!success)
        std::cerr << "Font \"" << name << "\" could not be added, an other font with the same name already exists." << std::endl;
    return success;
}




bool ResourceManager::loadMusicFromFile(const std::string path, const std::string name)
{
    sf::Music* music = new sf::Music();
    if (music->openFromFile(AUDIO_REPERTORY_NAME + path))
    {
        return addMusic(music, name);
    }
    std::cerr << "Music \"" << name << "\" could not be loaded." << std::endl;
    return false;
}

bool ResourceManager::loadSoundFromFile(const std::string path, const std::string name)
{
    sf::SoundBuffer* sound = new sf::SoundBuffer();
    if (sound->loadFromFile(AUDIO_REPERTORY_NAME + path))
    {
        return addSound(sound, name);
    }
    std::cerr << "Sound \"" << name << "\" could not be loaded." << std::endl;
    return false;
}

bool ResourceManager::loadTextureFromFile(const std::string path, const std::string name)
{
    sf::Texture* texture = new sf::Texture();
    if (texture->loadFromFile(TEXTURE_REPERTORY_NAME + path))
    {
        return addTexture(texture, name);
    }
    std::cerr << "Texture \"" << name << "\" could not be loaded." << std::endl;
    return false;
}

bool ResourceManager::loadFontFromFile(const std::string path, const std::string name)
{
    sf::Font* font = new sf::Font();
    if (font->loadFromFile(FONT_REPERTORY_NAME + path))
    {
        return addFont(font, name);
    }
    std::cerr << "Font \"" << name << "\" could not be loaded." << std::endl;
    return false;
}




sf::Music* ResourceManager::getMusic(const std::string name)
{
    auto i = musics.find(name);
    if (i == musics.end())
        std::cerr << "Music \"" << name << "\" could not be found." << std::endl;
    return (*i).second;
}

sf::SoundBuffer* ResourceManager::getSound(const std::string name)
{
    auto i = sounds.find(name);
    if (i == sounds.end())
        std::cerr << "Sound \"" << name << "\" could not be found." << std::endl;
    return (*i).second;
}

sf::Texture* ResourceManager::getTexture(const std::string name)
{
    auto i = textures.find(name);
    if (i == textures.end())
    {
        std::cerr << "Texture \"" << name << "\" could not be found." << std::endl;
        return getErrorTexture();
    }
    return (*i).second;
}

sf::Font* ResourceManager::getFont(const std::string name)
{
    auto i = fonts.find(name);
    if (i == fonts.end())
        std::cerr << "Font \"" << name << "\" could not be found." << std::endl;
    return (*i).second;
}
