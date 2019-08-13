#include "Chunk.h"

const size_t Chunk::CHUNK_SIZE = 8;

Chunk::Chunk(sf::Vector2i pos) :
    blocks(CHUNK_SIZE, CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE, CHUNK_SIZE, 0),
    pos(pos),
    ground_vertices(sf::Quads, 4 * CHUNK_SIZE * CHUNK_SIZE),
    block_side_vertices(sf::Quads),
    block_top_vertices(sf::Quads)
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
    //TODO
    //Temporary : colors
    for (size_t x = 0; x < CHUNK_SIZE; x++)
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 0].color = sf::Color::Red;
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 1].color = sf::Color::Red;
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 2].color = sf::Color::Yellow;
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 3].color = sf::Color::Green;
        }

    //Block
    //Sides
        //TODO

    //Tops
        //TODO

    vertices_ready = true;
}
