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
      vertices_ready(false),
      game(world.getGame()),
      world(world)
{
    for (int i = 0; i <= 1; ++i)
    {
        ground_vertices[i].setPrimitiveType(sf::Quads);
        ground_vertices[i].resize(4 * CHUNK_SIZE * CHUNK_SIZE);
        block_side_vertices[i].setPrimitiveType(sf::Quads);
        block_side_vertices[i].clear();
        block_top_vertices[i].setPrimitiveType(sf::Quads);
        block_top_vertices[i].clear();
    }

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
    for (int j = 0; j <= 1; ++j)
        for (int i = 0; i < 4; i++)
        {
            ground_detail_vertices[j].at(i).setPrimitiveType(sf::Quads);
            ground_detail_vertices[j].at(i).clear();
        }

    for (int i = 0; i < 4; i++)
        notifyChunk(i);

    success = true;
    ready = true;
}

Chunk::Chunk(World& world)  :
    blocks(CHUNK_SIZE*CHUNK_SIZE, 0),
    grounds(CHUNK_SIZE*CHUNK_SIZE, 0),
    tile_entities(CHUNK_SIZE*CHUNK_SIZE, nullptr),
    pos(sf::Vector2i()),
    vertices_ready(true),
    game(world.getGame()),
    world(world)
{
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
        world.getChunkConst(chunk).invalidateVertexArrays();
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

    invalidateVertexArrays();
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

    invalidateVertexArrays();
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
    assert(ground_vertices[gen_vx_id()].getVertexCount() != 0);
    vertices_ready = true;
    //log(INFO, "Chunk vertex array render took {}s.\n", clk.getElapsedTime().asSeconds());
}

void Chunk::generateGroundVertices() const
{
    auto& vertices = ground_vertices[gen_vx_id()];

    if (!ground_vertices_pos_ready)
    {
        for (int i = 0; i <= 1; ++i)
            for (int x = 0; x < CHUNK_SIZE; x++)
                for (int y = 0; y < CHUNK_SIZE; y++)
                {
                    ground_vertices[i][(x + y * CHUNK_SIZE) * 4 + 0].position = sf::Vector2f(pos.x * CHUNK_SIZE + x + -.5f, pos.y * CHUNK_SIZE + y + -.5f);
                    ground_vertices[i][(x + y * CHUNK_SIZE) * 4 + 1].position = sf::Vector2f(pos.x * CHUNK_SIZE + x + 0.5f, pos.y * CHUNK_SIZE + y + -.5f);
                    ground_vertices[i][(x + y * CHUNK_SIZE) * 4 + 2].position = sf::Vector2f(pos.x * CHUNK_SIZE + x + 0.5f, pos.y * CHUNK_SIZE + y + 0.5f);
                    ground_vertices[i][(x + y * CHUNK_SIZE) * 4 + 3].position = sf::Vector2f(pos.x * CHUNK_SIZE + x + -.5f, pos.y * CHUNK_SIZE + y + 0.5f);
                }
        ground_vertices_pos_ready = true;
    }
    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            TileReference tile(getBlockPosInWorld(x, y), world);

            Quad tr = tile.getGround()->getTextureVertices(tile);

            vertices[(x + y * CHUNK_SIZE) * 4 + 0].texCoords = tr.verts[0];
            vertices[(x + y * CHUNK_SIZE) * 4 + 1].texCoords = tr.verts[1];
            vertices[(x + y * CHUNK_SIZE) * 4 + 2].texCoords = tr.verts[2];
            vertices[(x + y * CHUNK_SIZE) * 4 + 3].texCoords = tr.verts[3];
        }
}

void Chunk::generateGroundDetailVertices() const
{
    for (int frame = 0; frame < 4; frame++)
    {
        auto& vertices = ground_detail_vertices[gen_vx_id()][frame];

        vertices.clear();
        for (size_t x = 0; x < CHUNK_SIZE; x++)
            for (size_t y = 0; y < CHUNK_SIZE; y++)
            {
                sf::Vector2i tile_pos = getBlockPosInWorld(x, y);
                TileReference tile(tile_pos, world);

                if (!tile.getGround()->hasSurfaceDetails(tile))
                    continue;

                auto details = tile.getGround()->getSurfaceDetails(tile, frame);

                for (size_t i = 0; i < details.getVertexCount(); i++)
                    vertices.append(details[i]);
            }
    }
}

void Chunk::generateBlockSideVertices() const
{
    auto& vertices = block_side_vertices[gen_vx_id()];
    vertices.clear();
    for (size_t x = 0; x < CHUNK_SIZE; x++)
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            TileReference tile(getBlockPosInWorld(x, y), world);

            if (tile.getBlock() == GameBlocks::AIR)
                continue;

            if (!tile.getBlock()->hasVolume(tile))
            {
                TextQuad plane = tile.getBlock()->getTopVertices(tile);

                for (size_t i = 0; i < 4; i++)
                    vertices.append(plane.verts[i]);

                continue;
            }

            TileReference tile_down = tile.getRelative(sf::Vector2i(0, 1));

            if (!tile.getBlock()->alwaysVisible() &&
                tile_down.getBlock()->occults(tile_down))
                continue;

            TextQuad side = tile.getBlock()->getSideVertices(tile);

            for (size_t i = 0; i < 4; i++)
                vertices.append(side.verts[i]);
        }
}

void Chunk::generateBlockTopVertices() const
{
    auto& vertices = block_top_vertices[gen_vx_id()];
    vertices.clear();
    for (size_t x = 0; x < CHUNK_SIZE; x++)
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            TileReference tile(getBlockPosInWorld(x, y), world);

            if (tile.getBlock() == GameBlocks::AIR)
                continue;

            if (!tile.getBlock()->hasVolume(tile))
                continue;

            TextQuad top = tile.getBlock()->getTopVertices(tile);

            for (size_t i = 0; i < 4; i++)
                vertices.append(top.verts[i]);
        }
}

