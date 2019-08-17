#pragma once

#include <cstdint>
#include <SFML/Graphics/Vertex.hpp>

#include "Quad.h"

class utils
{
    public:
        static constexpr int64_t combine(int32_t a, int32_t b)
        {
            return (((int64_t)a << 32) | b);
        }

        static TextQuad getSquare(Quad tex, sf::Vector2i pos)
        {
            TextQuad tq;
            tq.verts[0] = sf::Vertex(sf::Vector2f(-.5f + pos.x, -.5f + pos.y), tex.verts[0]);
            tq.verts[1] = sf::Vertex(sf::Vector2f(0.5f + pos.x, -.5f + pos.y), tex.verts[1]);
            tq.verts[2] = sf::Vertex(sf::Vector2f(0.5f + pos.x, 0.5f + pos.y), tex.verts[2]);
            tq.verts[3] = sf::Vertex(sf::Vector2f(-.5f + pos.x, 0.5f + pos.y), tex.verts[3]);
            return tq;
        }

        static sf::Vector2i getRelativeBlock(int direction)
        {
            switch (direction)
            {
            case 0:
                return sf::Vector2i(0, -1);
            case 1:
                return sf::Vector2i(1, 0);
            case 2:
                return sf::Vector2i(0, 1);
            case 3:
                return sf::Vector2i(-1, 0);
            default:
                return sf::Vector2i(0, 0);
            }
        }
};
