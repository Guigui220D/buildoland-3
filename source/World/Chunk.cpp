#include "Chunk.h"

#include "../Game.h"

#include "../Ground/Ground.h"
#include "../Block/Block.h"
#include "World.h"

//TMP
#include <cstdlib>

const size_t Chunk::CHUNK_SIZE = 8;

Chunk::Chunk(World* world, sf::Vector2i pos) :
    blocks(CHUNK_SIZE, CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE, CHUNK_SIZE, 0),
    pos(pos),
    ground_vertices(sf::Quads, 4 * CHUNK_SIZE * CHUNK_SIZE),
    ground_detail_vertices(sf::Quads),
    block_side_vertices(sf::Quads),
    block_top_vertices(sf::Quads),
    game(world->getGame()),
    world(world)
{
    regenerate();

    ready = true;
}

Chunk::~Chunk()
{
    //dtor
}

//For testing
void Chunk::regenerate()
{
    //Tmp
    for (size_t x = 0; x < CHUNK_SIZE; x++)
            for (size_t y = 0; y < CHUNK_SIZE; y++)
                grounds.set(x, y, std::rand() % 4);
    vertices_ready = false;
}

void Chunk::generateVertices() const
{
    generateGroundVertices();
    generateGroundDetailVertices();
    generateBlockSideVertices();
    generateBlockTopVertices();
    vertices_ready = true;
}

void Chunk::generateGroundVertices() const
{
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

            TextRect tr = ground->getTextureVertices(GroundInfo(world, ground_pos));

            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 0].texCoords = tr.verts[0];
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 1].texCoords = tr.verts[1];
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 2].texCoords = tr.verts[2];
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 3].texCoords = tr.verts[3];
        }
}

void Chunk::generateGroundDetailVertices() const
{
    ground_detail_vertices.clear();
    for (size_t x = 0; x < CHUNK_SIZE; x++)
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            sf::Vector2i ground_pos(x + pos.x * CHUNK_SIZE, y + pos.y * CHUNK_SIZE);
            const Ground* ground = game->getGroundsManager().getGroundByID(grounds.get(x, y));

            GroundInfo gi(world, ground_pos);

            if (!ground->hasSurfaceDetails(gi))
                continue;

            TextRect tr = ground->getSurfaceDetailVertices(gi);

            ground_detail_vertices.append(sf::Vertex(sf::Vector2f(-.5f + x + pos.x * CHUNK_SIZE, -.5f + y + pos.y * CHUNK_SIZE), tr.verts[0]));
            ground_detail_vertices.append(sf::Vertex(sf::Vector2f(0.5f + x + pos.x * CHUNK_SIZE, -.5f + y + pos.y * CHUNK_SIZE), tr.verts[1]));
            ground_detail_vertices.append(sf::Vertex(sf::Vector2f(0.5f + x + pos.x * CHUNK_SIZE, 0.5f + y + pos.y * CHUNK_SIZE), tr.verts[2]));
            ground_detail_vertices.append(sf::Vertex(sf::Vector2f(-.5f + x + pos.x * CHUNK_SIZE, 0.5f + y + pos.y * CHUNK_SIZE), tr.verts[3]));
        }
}

void Chunk::generateBlockSideVertices() const
{

}

void Chunk::generateBlockTopVertices() const
{

}

