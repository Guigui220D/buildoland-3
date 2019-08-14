#include "Chunk.h"

#include "../Game.h"

#include "../Ground/Ground.h"
#include "../Block/Block.h"

const size_t Chunk::CHUNK_SIZE = 8;

Chunk::Chunk(Game* game, sf::Vector2i pos) :
    blocks(CHUNK_SIZE, CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE, CHUNK_SIZE, 0),
    pos(pos),
    ground_vertices(sf::Quads, 4 * CHUNK_SIZE * CHUNK_SIZE),
    block_side_vertices(sf::Quads),
    block_top_vertices(sf::Quads),
    game(game)
{
    ready = true;
}

Chunk::~Chunk()
{
    //dtor
}

void Chunk::generateVertices() const
{
    //Ground
    if (!ground_vertices_pos_ready)
    {
        for (size_t x = 0; x < CHUNK_SIZE; x++)
            for (size_t y = 0; y < CHUNK_SIZE; y++)
            {
                ground_vertices[(x + y * CHUNK_SIZE) * 4 + 0].position = sf::Vector2f(-.5f + x + pos.x * CHUNK_SIZE, -.5f + y + pos.y * CHUNK_SIZE);
                ground_vertices[(x + y * CHUNK_SIZE) * 4 + 1].position = sf::Vector2f(0.5f + x + pos.x * CHUNK_SIZE, -.5f + y + pos.y * CHUNK_SIZE);
                ground_vertices[(x + y * CHUNK_SIZE) * 4 + 2].position = sf::Vector2f(0.5f + x + pos.x * CHUNK_SIZE, 0.5f + y + pos.y * CHUNK_SIZE);
                ground_vertices[(x + y * CHUNK_SIZE) * 4 + 3].position = sf::Vector2f(-.5f + x + pos.x * CHUNK_SIZE, 0.5f + y + pos.y * CHUNK_SIZE);
            }
        ground_vertices_pos_ready = true;
    }
    for (size_t x = 0; x < CHUNK_SIZE; x++)
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            sf::Vector2i ground_pos(x + pos.x * CHUNK_SIZE, y + pos.y * CHUNK_SIZE);
            const Ground* ground = game->getGroundsManager().getGroundByID(grounds.get(x, y));
            TextRect tr = ground->getTextureVertices(ground_pos);
            uint8_t rot = ground->getTextureRotation(ground_pos);
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + (0 + rot) % 4].texCoords = tr.vertA;
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + (1 + rot) % 4].texCoords = tr.vertB;
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + (2 + rot) % 4].texCoords = tr.vertC;
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + (3 + rot) % 4].texCoords = tr.vertD;
        }

    //Block
    //Sides
        //TODO

    //Tops
        //TODO

    vertices_ready = true;
}
