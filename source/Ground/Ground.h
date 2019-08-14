#pragma once

#include <string>
#include <SFML/System.hpp>

class Ground
{
    public:
        Ground(const std::string name);
        virtual ~Ground();

        inline const std::string getName() const { return name; }

        virtual inline uint32_t getTextureNumber(sf::Vector2i ground_pos) const { return 0; }
        virtual inline uint8_t getTextureRotation(sf::Vector2i ground_pos) const { return 0; }

        //TEMPORARY
        inline sf::Vector2f getTexCoordA(sf::Vector2i ground_pos) const
        {
            uint32_t tn = getTextureNumber(ground_pos);
            return sf::Vector2f((tn % TILESET_WIDTH) * TEXTURE_WIDTH, (tn / TILESET_WIDTH) * TEXTURE_WIDTH);
        }
        inline sf::Vector2f getTexCoordB(sf::Vector2i ground_pos) const
        {
            uint32_t tn = getTextureNumber(ground_pos);
            return sf::Vector2f((tn % TILESET_WIDTH) * TEXTURE_WIDTH + TEXTURE_WIDTH, (tn / TILESET_WIDTH) * TEXTURE_WIDTH);
        }
        inline sf::Vector2f getTexCoordC(sf::Vector2i ground_pos) const
        {
            uint32_t tn = getTextureNumber(ground_pos);
            return sf::Vector2f((tn % TILESET_WIDTH) * TEXTURE_WIDTH + TEXTURE_WIDTH, (tn / TILESET_WIDTH) * TEXTURE_WIDTH + TEXTURE_WIDTH);
        }
        inline sf::Vector2f getTexCoordD(sf::Vector2i ground_pos) const
        {
            uint32_t tn = getTextureNumber(ground_pos);
            return sf::Vector2f((tn % TILESET_WIDTH) * TEXTURE_WIDTH, (tn / TILESET_WIDTH) * TEXTURE_WIDTH + TEXTURE_WIDTH);
        }

    private:
        static const size_t TILESET_WIDTH;
        static const size_t TEXTURE_WIDTH;

        std::string name;
};
