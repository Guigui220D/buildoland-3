#include "Chunk.h"

#include "../Game.h"

#include "../Ground/Ground.h"
#include "../Block/Block.h"
#include "World.h"

#include <assert.h>

const int Chunk::CHUNK_SIZE = 16;

Chunk::Chunk(World* world, sf::Vector2i pos, sf::Packet& packet, bool& success) :
    blocks(CHUNK_SIZE, CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE, CHUNK_SIZE, 0),
    pos(pos),
    ground_vertices(sf::Quads, 4 * CHUNK_SIZE * CHUNK_SIZE),
    block_side_vertices(sf::Quads),
    block_top_vertices(sf::Quads),
    game(world->getGame()),
    world(world)
{
    size_t header_size = sizeof(int) * 2 + 2;

    if (packet.getDataSize() < getChunkDataSize() + header_size)
    {
        success = false;
        return;
    }

    const char* data = (const char*)packet.getData();

    //Copying data
    memcpy(blocks.getData(), data + header_size, blocks.getDataSize());
    memcpy(grounds.getData(), data + header_size + blocks.getDataSize(), grounds.getDataSize());


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
    if (world->isChunkLoaded(chunk))
        world->getChunkConst(chunk).mustRedoVertexArrays();
}

const Block* Chunk::getBlock(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    return game->getBlocksManager().getBlockByID(getBlockId(x, y));
}

const Ground* Chunk::getGround(int x, int y) const
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    return game->getGroundsManager().getGroundByID(getGroundId(x, y));
}


void Chunk::setBlock(int x, int y, uint16_t id)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    blocks.set(x, y, id);
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

void Chunk::setGround(int x, int y, uint16_t id)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    grounds.set(x, y, id);
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

void Chunk::setBlock(int x, int y, const Block* block)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    setBlock(x, y, block->getId());
}

void Chunk::setGround(int x, int y, const Ground* ground)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < CHUNK_SIZE);
    assert(y < CHUNK_SIZE);
    setGround(x, y, ground->getId());
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
    std::cout << "Chunk vertex array render took " << clk.getElapsedTime().asSeconds() << "s" << std::endl;
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
            const Block* block_down = world->getBlock(block_pos + sf::Vector2i(0, 1));

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

