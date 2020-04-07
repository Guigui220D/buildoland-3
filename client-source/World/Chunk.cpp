#include "Chunk.h"

#include "../Game.h"

#include "../../common-source/Grounds/Ground.h"
#include "../../common-source/Blocks/Block.h"
#include "../../common-source/Utils/Log.h"
#include "World.h"

#include <assert.h>

const int Chunk::CHUNK_SIZE = 16;

Chunk::Chunk(World& world, sf::Vector2i pos, const char* chunk_data, unsigned chunk_data_size, bool& success) :
    blocks(CHUNK_SIZE*CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE*CHUNK_SIZE, 0),
    tile_entities(CHUNK_SIZE*CHUNK_SIZE, nullptr),
    pos(pos),
    ground_vertices(sf::Quads, 4 * CHUNK_SIZE * CHUNK_SIZE),
    block_side_vertices(sf::Quads),
    block_top_vertices(sf::Quads),
    game(world.getGame()),
    world(world)
{
    size_t header_size = sizeof(int) * 3;

    if (chunk_data_size < getChunkDataSize() + header_size)
    {
        success = false;
        return;
    }

    //Copying data
    memcpy(blocks.data(), chunk_data + header_size, blocks.size()*sizeof(blocks[0]));
    memcpy(grounds.data(), chunk_data + header_size + blocks.size()*sizeof(blocks[0]), grounds.size()*sizeof(grounds[0]));


    //Prepare vertices
    for (int i = 0; i < 4; i++)
    {
        ground_detail_vertices.at(i).setPrimitiveType(sf::Quads);
        ground_detail_vertices.at(i).clear();
    }

    for (int i = 0; i < 4; i++)
        notifyChunk(i);

    success = true;
    ready = true;
}

Chunk::~Chunk()
{
    //dtor
}

void Chunk::notifyChunk(int direction) const
{
    assert(direction >= 0 && direction < 4);
    sf::Vector2i chunk = pos + utils::getRelativeBlock(direction);
    if (world.isChunkLoaded(chunk))
        world.getChunkConst(chunk).mustRedoVertexArrays();
}

const Block* Chunk::getBlock(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    return game.getBlocksManager().getBlockByID(getBlockId(x, y));
}

const Ground* Chunk::getGround(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    return game.getGroundsManager().getGroundByID(getGroundId(x, y));
}

void Chunk::setBlock(int x, int y, const Block* block)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);

    blocks[y*CHUNK_SIZE + x] = block->getId();

    if (x == 0)
        notifyChunk(3);
    if (y == 0)
        notifyChunk(0);
    if (x == CHUNK_SIZE - 1)
        notifyChunk(1);
    if (y == CHUNK_SIZE - 1)
        notifyChunk(2);

    vertices_ready = false;
}

void Chunk::setGround(int x, int y, const Ground* ground)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);

    grounds[y*CHUNK_SIZE + x] = ground->getId();

    if (x == 0)
        notifyChunk(3);
    if (y == 0)
        notifyChunk(0);
    if (x == CHUNK_SIZE - 1)
        notifyChunk(1);
    if (y == CHUNK_SIZE - 1)
        notifyChunk(2);

    vertices_ready = false;
}

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
    //log(INFO, "Chunk vertex array render took {}s.\n", clk.getElapsedTime().asSeconds());
}

void Chunk::generateGroundVertices() const
{
    if (!ground_vertices_pos_ready)
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                ground_vertices[(x + y * CHUNK_SIZE) * 4 + 0].position = sf::Vector2f(pos.x * CHUNK_SIZE + x + -.5f, pos.y * CHUNK_SIZE + y + -.5f);
                ground_vertices[(x + y * CHUNK_SIZE) * 4 + 1].position = sf::Vector2f(pos.x * CHUNK_SIZE + x + 0.5f, pos.y * CHUNK_SIZE + y + -.5f);
                ground_vertices[(x + y * CHUNK_SIZE) * 4 + 2].position = sf::Vector2f(pos.x * CHUNK_SIZE + x + 0.5f, pos.y * CHUNK_SIZE + y + 0.5f);
                ground_vertices[(x + y * CHUNK_SIZE) * 4 + 3].position = sf::Vector2f(pos.x * CHUNK_SIZE + x + -.5f, pos.y * CHUNK_SIZE + y + 0.5f);
            }
        ground_vertices_pos_ready = true;
    }
    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int y = 0; y < CHUNK_SIZE; y++)
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

                sf::Vector2i tile_pos = getBlockPosInWorld(x, y);

                GroundInfo gi(world, tile_pos);
                BlockInfo bi(world, tile_pos);

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
            const Block* block_down = world.getBlock(block_pos + sf::Vector2i(0, 1));

            BlockInfo bi(world, block_pos);
            BlockInfo bi_down(world, block_pos + sf::Vector2i(0, 1));

            if (block == GameBlocks::AIR)
                continue;

            if (!block->hasVolume(bi))
            {
                TextQuad plane = block->getTopVertices(bi);

                for (size_t i = 0; i < 4; i++)
                    block_side_vertices.append(plane.verts[i]);

                continue;
            }

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

            if (!block->hasVolume(bi))
                continue;

            TextQuad top = block->getTopVertices(bi);

            for (size_t i = 0; i < 4; i++)
                block_top_vertices.append(top.verts[i]);
        }
}

