#include "Chunk.h"

#include "../Game.h"

#include "../Ground/Ground.h"
#include "../Block/Block.h"
#include "World.h"

//TMP
#include <cstdlib>

const size_t Chunk::CHUNK_SIZE = 16;

Chunk::Chunk(World* world, sf::Vector2i pos) :
    blocks(CHUNK_SIZE, CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE, CHUNK_SIZE, 0),
    pos(pos),
    ground_vertices(sf::Quads, 4 * CHUNK_SIZE * CHUNK_SIZE),
    block_side_vertices(sf::Quads),
    block_top_vertices(sf::Quads),
    game(world->getGame()),
    world(world)
{
    for (int i = 0; i < 4; i++)
    {
        ground_detail_vertices.at(i).setPrimitiveType(sf::Quads);
        ground_detail_vertices.at(i).clear();
    }

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
    vertices_ready = false;

    uint16_t tree_x, tree_y;
    tree_x = std::rand() % 8;
    tree_y = std::rand() % 8;
    //Tmp
    for (size_t x = 0; x < CHUNK_SIZE; x++)
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            grounds.set(x, y, std::rand() % 4);
            blocks.set(x, y, 0);
            if (getGround(x, y) != GameGrounds::WATER && !(std::rand() % 5))
                blocks.set(x, y, std::rand() % 5);
        }
    setBlock(tree_x, tree_y, GameBlocks::TREE);
    setGround(tree_x, tree_y, GameGrounds::GRASS);

    for (int i = 0; i < 4; i++)
    {
        sf::Vector2i chunk = pos + utils::getRelativeBlock(i);
        if (world->isChunkLoaded(chunk))
            world->getChunk(chunk).mustRedoVertexArrays();
    }
}

const Block* Chunk::getBlock(int x, int y) const
    { return game->getBlocksManager().getBlockByID(getBlockId(x, y)); }

const Ground* Chunk::getGround(int x, int y) const
    { return game->getGroundsManager().getGroundByID(getGroundId(x, y)); }

void Chunk::setBlock(int x, int y, const Block* block)
    { blocks.set(x, y, block->getId()); }

void Chunk::setGround(int x, int y, const Ground* ground)
    { grounds.set(x, y, ground->getId()); }

void Chunk::generateVertices() const
{
    sf::Clock clk;
    if (!ready)
        return;
    generateGroundVertices();
    generateGroundDetailVertices();
    generateBlockSideVertices();
    generateBlockTopVertices();
    vertices_ready = true;
    std::cout << "Chunk vertex array render took " << clk.getElapsedTime().asSeconds() << "s" << std::endl;
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
            const Ground* ground = getGround(x, y);

            Quad tr = ground->getTextureVertices(GroundInfo(world, getBlockPosInWorld(x, y)));

            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 0].texCoords = tr.verts[0];
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 1].texCoords = tr.verts[1];
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 2].texCoords = tr.verts[2];
            ground_vertices[(x + y * CHUNK_SIZE) * 4 + 3].texCoords = tr.verts[3];
        }
}

void Chunk::generateGroundDetailVertices() const
{
    for (int frame = 0; frame < 4; frame++)
    {
        ground_detail_vertices[frame].clear();
        for (size_t x = 0; x < CHUNK_SIZE; x++)
            for (size_t y = 0; y < CHUNK_SIZE; y++)
            {
                const Ground* ground = getGround(x, y);

                GroundInfo gi(world, getBlockPosInWorld(x, y));

                if (!ground->hasSurfaceDetails(gi))
                    continue;

                auto details = ground->getSurfaceDetails(gi, frame);

                for (size_t i = 0; i < details.getVertexCount(); i++)
                    ground_detail_vertices[frame].append(details[i]);
            }
    }
}

void Chunk::generateBlockSideVertices() const
{
    block_side_vertices.clear();
    for (size_t x = 0; x < CHUNK_SIZE; x++)
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            sf::Vector2i block_pos = getBlockPosInWorld(x, y);
            const Block* block = getBlock(x, y);
            const Block* block_down = world->getBlock(block_pos + sf::Vector2i(0, 1), false);

            BlockInfo bi(world, block_pos);
            BlockInfo bi_down(world, block_pos + sf::Vector2i(0, 1));

            if (block == GameBlocks::AIR)
                continue;

            if (!block->hasVolume(bi))
                continue;

            if (!block->alwaysVisible() && block_down->occults(bi_down))
                continue;

            TextQuad side = block->getSideVertices(bi);

            for (size_t i = 0; i < 4; i++)
                block_side_vertices.append(side.verts[i]);
        }
}

void Chunk::generateBlockTopVertices() const
{
    block_top_vertices.clear();
    for (size_t x = 0; x < CHUNK_SIZE; x++)
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            const Block* block = getBlock(x, y);

            BlockInfo bi(world, getBlockPosInWorld(x, y));

            if (block == GameBlocks::AIR)
                continue;

            TextQuad top = block->getTopVertices(bi);

            for (size_t i = 0; i < 4; i++)
                block_top_vertices.append(top.verts[i]);
        }
}

