#pragma once

namespace sf
{
class Music;
class SoundBuffer;
class Texture;
class Font;
}

#include <unordered_map>
#include <memory>

class ResourceManager
{
    public:
        static const std::string AUDIO_REPERTORY_NAME;
        static const std::string FONT_REPERTORY_NAME;
        static const std::string TEXTURE_REPERTORY_NAME;

        ResourceManager();
        ~ResourceManager();

        //Add assets
        /**
         * Directly add a music to this resource manager
         * @param music : a pointer to the music to add
         * @param name : the name to find this resource easily later
         * @return True if success in inserting the resource
         */
        bool addMusic(sf::Music* music, const std::string& name);
        /**
         * Directly add a sound to this resource manager
         * @param sound : a pointer to the sound to add
         * @param name : the name to find this resource easily later
         * @return True if success in inserting the resource
         */
        bool addSound(sf::SoundBuffer* sound, const std::string& name);
        /**
         * Directly add a texture to this resource manager
         * @param texture : a pointer to the texture to add
         * @param name : the name to find this resource easily later
         * @return True if success in inserting the resource
         */
        bool addTexture(sf::Texture* texture, const std::string& name);
        /**
         * Directly add a font to this resource manager
         * @param font : a pointer to the font to add
         * @param name : the name to find this resource easily later
         * @return True if success in inserting the resource
         */
        bool addFont(sf::Font* font, const std::string& name);

        //Add assets from files
        /**
         * Loads a music from file and adds it to this resource manager
         * @param path : a path to the file to load from
         * @param name : the name to find this resource easily later
         * @return True if success in loading the file and inserting it
         */
        bool loadMusicFromFile(const std::string& path, const std::string& name);
        /**
         * Loads a sound from file and adds it to this resource manager
         * @param path : a path to the file to load from
         * @param name : the name to find this resource easily later
         * @return True if success in loading the file and inserting it
         */
        bool loadSoundFromFile(const std::string& path, const std::string& name);
        /**
         * Loads a texture from file and adds it to this resource manager
         * @param path : a path to the file to load from
         * @param name : the name to find this resource easily later
         * @return True if success in loading the file and inserting it
         */
        bool loadTextureFromFile(const std::string& path, const std::string& name);
        /**
         * Loads a font from file and adds it to this resource manager
         * @param path : a path to the file to load from
         * @param name : the name to find this resource easily later
         * @return True if success in loading the file and inserting it
         */
        bool loadFontFromFile(const std::string& path, const std::string& name);

        //Get assets
        /**
         * Gets a pointer to a texture using its name
         * @param name : the name used previously to register the resource
         * @return The pointer to the resource
         */
        const sf::Texture& getTexture(const std::string name) const;
        /**
         * Gets a pointer to a font using its name
         * @param name : the name used previously to register the resource
         * @return The pointer to the resource
         */
        const sf::Font& getFont(const std::string name) const;

        /**
         * Gets the default checkerboard texture in case of error
         * @return A pointer to the checkerboard texture
         */
        inline const sf::Texture& getErrorTexture() const { return *error_texture; };

    private:
        friend class AudioManager;
        //Get audio
        //These are private because they should only be accessed through Audio manager which is a friend
        /**
         * Gets a pointer to a music using its name
         * @param name : the name used previously to register the resource
         * @return The pointer to the resource
         */
        sf::Music& getMusic(const std::string& name);
        /**
         * Gets a pointer to a sound using its name
         * @param name : the name used previously to register the resource
         * @return The pointer to the resource
         */
        sf::SoundBuffer& getSound(const std::string& name);

        std::unordered_map<std::string, sf::Music*> musics;
        std::unordered_map<std::string, sf::SoundBuffer*> sounds;
        std::unordered_map<std::string, sf::Texture*> textures;
        std::unordered_map<std::string, sf::Font*> fonts;

        std::unique_ptr<sf::Texture> error_texture;
        std::unique_ptr<sf::Music> error_music;
        std::unique_ptr<sf::SoundBuffer> error_sound;
        std::unique_ptr<sf::Font> error_font;
};
