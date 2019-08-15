#pragma once

#include <SFML/Graphics.hpp>
#include "Quad.h"

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
class TilesetHelper
{
    public:
        TilesetHelper();

        sf::FloatRect getFloatRect(unsigned int number) const;
        Quad getFourVertices(unsigned int number, uint8_t rotation = 0) const;

    private:
};

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
TilesetHelper<TextureSize, TilesetWidth, MarginSize>::TilesetHelper()
{ }

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
sf::FloatRect TilesetHelper<TextureSize, TilesetWidth, MarginSize>::getFloatRect(unsigned int number) const
{
    return sf::FloatRect((number % TilesetWidth) * TextureSize, (number / TilesetWidth) * TextureSize, TextureSize, TextureSize);
}

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
Quad TilesetHelper<TextureSize, TilesetWidth, MarginSize>::getFourVertices(unsigned int number, uint8_t rotation) const
{
    Quad textRect;
    sf::FloatRect rect = getFloatRect(number);
    textRect.verts[(0 + rotation) % 4] = sf::Vector2f(rect.left, rect.top);
    textRect.verts[(1 + rotation) % 4] = sf::Vector2f(rect.left + rect.width, rect.top);
    textRect.verts[(2 + rotation) % 4] = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
    textRect.verts[(3 + rotation) % 4] = sf::Vector2f(rect.left, rect.top + rect.height);
    return textRect;
}
