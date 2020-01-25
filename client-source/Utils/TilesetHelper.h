#pragma once

#include <SFML/Graphics.hpp>
#include "Quad.h"

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
class TilesetHelper
{
    public:
        TilesetHelper();

        sf::FloatRect getFloatRect(unsigned int number, unsigned int width = 1, unsigned int height = 1) const;
        sf::IntRect getIntRect(unsigned int number, unsigned int width = 1, unsigned int height = 1) const;
        Quad getFourVertices(unsigned int number, uint8_t rotation = 0, unsigned int width = 1, unsigned int height = 1) const;

    private:
};

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
TilesetHelper<TextureSize, TilesetWidth, MarginSize>::TilesetHelper()
{ }

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
sf::FloatRect TilesetHelper<TextureSize, TilesetWidth, MarginSize>::getFloatRect(unsigned int number, unsigned int width, unsigned int height) const
{
    return sf::FloatRect(
        (number % TilesetWidth) * (TextureSize + MarginSize * 2) + MarginSize,
        (number / TilesetWidth) * (TextureSize + MarginSize * 2) + MarginSize,
        (TextureSize + MarginSize * 2) * width - MarginSize * 2,
        (TextureSize + MarginSize * 2) * height - MarginSize * 2
    );
}

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
sf::IntRect TilesetHelper<TextureSize, TilesetWidth, MarginSize>::getIntRect(unsigned int number, unsigned int width, unsigned int height) const
{
    return sf::IntRect(
        (number % TilesetWidth) * (TextureSize + MarginSize * 2) + MarginSize,
        (number / TilesetWidth) * (TextureSize + MarginSize * 2) + MarginSize,
        (TextureSize + MarginSize * 2) * width - MarginSize * 2,
        (TextureSize + MarginSize * 2) * height - MarginSize * 2
    );
}

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
Quad TilesetHelper<TextureSize, TilesetWidth, MarginSize>::getFourVertices(unsigned int number, uint8_t rotation, unsigned int width, unsigned int height) const
{
    Quad textRect;
    sf::FloatRect rect = getFloatRect(number, width, height);
    textRect.verts[(0 + rotation) % 4] = sf::Vector2f(rect.left, rect.top);
    textRect.verts[(1 + rotation) % 4] = sf::Vector2f(rect.left + rect.width, rect.top);
    textRect.verts[(2 + rotation) % 4] = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
    textRect.verts[(3 + rotation) % 4] = sf::Vector2f(rect.left, rect.top + rect.height);
    return textRect;
}
