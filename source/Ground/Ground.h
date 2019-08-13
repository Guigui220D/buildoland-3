#pragma once

#include <string>
#include <SFML/System.hpp>

class Ground
{
    public:
        Ground(const std::string name);
        virtual ~Ground();

        inline const std::string getName() const { return name; }

        virtual uint32_t getTextureNumber(sf::Vector2i ground_pos) const;
        virtual uint8_t getTextureRotation(sf::Vector2i ground_pos) const;

    private:
        std::string name;
};
