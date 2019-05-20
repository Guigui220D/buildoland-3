#include "AssetManager.h"

const std::string AssetManager::AUDIO_REPERTORY_NAME = "Res/";
const std::string AssetManager::TEXTURE_REPERTORY_NAME = "Res/";
const std::string AssetManager::FONT_REPERTORY_NAME = "Res/";

AssetManager::AssetManager(unsigned int maxSounds)
{
    maxSoundsPlaying = maxSounds;

    sf::Image errorImage;
    errorImage.create(2, 2);
    errorImage.setPixel(0, 0, sf::Color(255, 0, 255));
    errorImage.setPixel(1, 1, sf::Color(255, 0, 255));
    errorTexture.loadFromImage(errorImage);
}

AssetManager::~AssetManager()
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

void AssetManager::addPlayingSound(sf::Sound* sound)
{
    playingSounds.push_back(sound);
    if (playingSounds.size() > maxSoundsPlaying)
    {
        delete playingSounds.at(0);
        playingSounds.erase(playingSounds.begin());
    }
}

void AssetManager::addMusic(sf::Music* music, const std::string name)
{
    musics.emplace(std::pair<std::string, sf::Music*>(name, music));
}
void AssetManager::addSound(sf::SoundBuffer* sound, const std::string name)
{
    sounds.emplace(std::pair<std::string, sf::SoundBuffer*>(name, sound));
}
void AssetManager::addTexture(sf::Texture* texture, const std::string name)
{
    textures.emplace(std::pair<std::string, sf::Texture*>(name, texture));
}
void AssetManager::addFont(sf::Font* font, const std::string name)
{
    fonts.emplace(std::pair<std::string, sf::Font*>(name, font));
}

bool AssetManager::loadMusicFromFile(const std::string path, const std::string name)
{
    sf::Music* music = new sf::Music();
    if (music->openFromFile(AUDIO_REPERTORY_NAME + path))
    {
        musics.emplace(std::pair<std::string, sf::Music*>(name, music));
        return true;
    }
    std::cout << "Music \"" << name << "\" could not be loaded.\n";
    return false;
}
bool AssetManager::loadSoundFromFile(const std::string path, const std::string name)
{
    sf::SoundBuffer* sound = new sf::SoundBuffer();
    if (sound->loadFromFile(AUDIO_REPERTORY_NAME + path))
    {
        sounds.emplace(std::pair<std::string, sf::SoundBuffer*>(name, sound));
        return true;
    }
    std::cout << "Sound \"" << name << "\" could not be loaded.\n";
    return false;
}
bool AssetManager::loadTextureFromFile(const std::string path, const std::string name)
{
    sf::Texture* texture = new sf::Texture();
    if (texture->loadFromFile(TEXTURE_REPERTORY_NAME + path))
    {
        textures.emplace(std::pair<std::string, sf::Texture*>(name, texture));
        return true;
    }
    std::cout << "Texture \"" << name << "\" could not be loaded.\n";
    return false;
}
bool AssetManager::loadFontFromFile(const std::string path, const std::string name)
{
    sf::Font* font = new sf::Font();
    if (font->loadFromFile(FONT_REPERTORY_NAME + path))
    {
        fonts.emplace(std::pair<std::string, sf::Font*>(name, font));
        return true;
    }
    std::cout << "Font \"" << name << "\" could not be loaded.\n";
    return false;
}

sf::Music* AssetManager::getMusic(const std::string name)
{
    auto i = musics.find(name);
    if (i == musics.end())
        std::cout << "Music \"" << name << "\" could not be found.\n";
    return (*i).second;
}
sf::SoundBuffer* AssetManager::getSound(const std::string name)
{
    auto i = sounds.find(name);
    if (i == sounds.end())
        std::cout << "Sound \"" << name << "\" could not be found.\n";
    return (*i).second;
}
sf::Texture* AssetManager::getTexture(const std::string name)
{
    auto i = textures.find(name);
    if (i == textures.end())
    {
        std::cout << "Texture \"" << name << "\" could not be found.\n";
        return getErrorTexture();
    }
    return (*i).second;
}
sf::Font* AssetManager::getFont(const std::string name)
{
    auto i = fonts.find(name);
    if (i == fonts.end())
        std::cout << "Font \"" << name << "\" could not be found.\n";
    return (*i).second;
}
