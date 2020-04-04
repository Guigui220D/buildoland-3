#include "World.h"

#include "../Utils/Utils.h"
#include "../States/GameState.h"
#include "../Game.h"
#include "../../common-source/Networking/ClientToServerCodes.h"
#include "../../common-source/Constants.h"
#include "../../common-source/Entities/GameEntities/Player.h"

#include "../Packets/ChunkRequestPacket.h"

#include <iostream>

int unsigned World::RENDER_DISTANCE = 3;

World::World(GameState& state_game) :
    entities(*this),
    state_game(state_game),
    game(state_game.getGame()),
    game_blocks_manager(state_game.getGame().getBlocksManager()),
    game_grounds_manager(state_game.getGame().getGroundsManager())
{
    std::srand(time(0));
    seed = std::rand() << 16 | std::rand();
}

World::World(GameState& state_game, int seed) :
    entities(*this),
    state_game(state_game),
    game(state_game.getGame()),
    game_blocks_manager(state_game.getGame().getBlocksManager()),
    game_grounds_manager(state_game.getGame().getGroundsManager()),
    seed(seed)
{
}

World::~World()
{
    for (Chunk*& chunk : chunks_to_add)
        delete chunk;
}

void World::updateLoadedChunk(float delta_time)
{
    //Add chunks waiting to be added
    for (Chunk*& chunk : chunks_to_add)
    {
        uint64_t key = utils::combine(chunk->getPos().x, chunk->getPos().y);

        if (chunks.find(key) != chunks.end())
            chunks.erase(chunks.find(key));

        chunks.emplace(std::pair<uint64_t, std::unique_ptr<Chunk>>(key, std::unique_ptr<Chunk>(chunk)));

        entities.declareNewChunkForTileEntities(chunk);
    }
    chunks_to_add.clear();

    for (auto i = pending_chunk_requests.begin(); i != pending_chunk_requests.end();)
    {
        if (i->second.getElapsedTime().asSeconds() >= 1.f) // timeout; re-request the block
        {
            sf::Vector2i pos;
            utils::unpack(i->first, pos.x, pos.y);

            sf::Vector2i diff = player_chunk_pos - pos;

            int distance_squared = diff.x * diff.x + diff.y * diff.y;

            if (distance_squared >= Constants::CHUNK_LOADING_DISTANCE)
            {
                i = pending_chunk_requests.erase(i);
                continue;
            }

            requestChunk(pos);
            std::cerr << "Outdated request for chunk " << pos.x << "; " << pos.y << " !\n";
        }
        i++;
    }

    //Unload all the chunks that need to be removed
    for (auto i = chunks.begin(); i != chunks.end();)
    {
        if (i->second->to_be_removed)
        {
            //Kills entities in that chunk
            for (auto& entity : entities.entities_map)
            {
                if (entity.second->getId() == Player::this_player_id)
                    continue;

                if (entity.second->getChunkOn() == i->second->getPos())
                    entity.second->to_be_removed = true;
            }

            //Removes the chunk from the map
            i = chunks.erase(i);
        }
        else
            i++;
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
        if (pending_chunk_requests.find(utils::combine(player_chunk_pos.x, player_chunk_pos.y)) == pending_chunk_requests.end())
            requestChunk(player_chunk_pos);

}

bool World::addChunk(ECCPacket& packet)
{
    //We expect the packet to be of that size
    //4 bytes per tile (2 for block and 2 for ground)
    //8 bytes for the position
    //2 bytes for the packet header
    size_t expected_packet_size = Chunk::getChunkDataSize();
    expected_packet_size += sizeof(int) * 2; //Position
    expected_packet_size += 2;

    if (packet.getDataSize() < expected_packet_size)
    {
        std::cerr << "Chunk packet is too small! Expected " << expected_packet_size << " bytes, got " << packet.getDataSize() << " bytes." << std::endl;
        return false;
    }

    //Get chunk position
    sf::Vector2i pos;
    packet >> pos.x;
    packet >> pos.y;

    std::cout << "New chunk has position " << pos.x << ", " << pos.y << std::endl;

    sf::Vector2i diff = pos - player_chunk_pos;
    int distance_squared = diff.x * diff.x + diff.y * diff.y;
    if (distance_squared >= Constants::CHUNK_LOADING_DISTANCE)
        return true;    //Ignore chunk because it's too far

    //Construct new chunk
    bool success = false;

    Chunk* new_chunk = new Chunk(*this, pos, packet, success);

    if (!success)
    {
        std::cerr << "A new chunk packet couldn't be parsed" << std::endl;
        delete new_chunk;
        return false;
    }

    auto it = pending_chunk_requests.find(utils::combine(pos.x, pos.y));
    if (it != pending_chunk_requests.end())
        pending_chunk_requests.erase(it);

    //Add the new chunk!
    chunks_to_add.push_back(new_chunk);
    return true;
}

Chunk& World::getChunk(sf::Vector2i pos)
{
    uint64_t key = utils::combine(pos.x, pos.y);
    auto chunk_ptr = chunks.find(key);

    if (chunk_ptr == chunks.end())
        throw new std::out_of_range("World::getChunk : tried to access chunk that doesn't exist.");

    return *chunk_ptr->second;
}

const Chunk& World::getChunkConst(sf::Vector2i pos) const
{
    uint64_t key = utils::combine(pos.x, pos.y);
    auto chunk_ptr = chunks.find(key);

    if (chunk_ptr == chunks.end())
        throw new std::out_of_range("World::getChunkConst : tried to access chunk that doesn't exist.");

    return *chunk_ptr->second;
}

void World::requestChunk(sf::Vector2i pos)
{
    ChunkRequestPacket request(pos);

    state_game.sendToServer(request);

    pending_chunk_requests[utils::combine(pos.x, pos.y)].restart(); // chunk request timeout : 1 second
}

void World::updateChunks(sf::Vector2i center)
{
    player_chunk_pos = center;

    for (auto& chunk : chunks)
    {
        sf::Vector2i diff = chunk.second->getPos() - center;
        int distance_squared = diff.x * diff.x + diff.y * diff.y;

        //TODO : Make render distance constant
        if (distance_squared >= Constants::CHUNK_LOADING_DISTANCE)
            chunk.second->to_be_removed = true;
    }

    for (int x = -Constants::CHUNK_LOADING_DISTANCE; x <= Constants::CHUNK_LOADING_DISTANCE; x++)
    for (int y = -Constants::CHUNK_LOADING_DISTANCE; y <= Constants::CHUNK_LOADING_DISTANCE; y++)
    {
        int distance_squared = x * x + y * y;

        if (distance_squared < Constants::CHUNK_LOADING_DISTANCE)
        {
            sf::Vector2i pos = sf::Vector2i(x, y) + center;

            if (!isChunkLoaded(pos))
                requestChunk(pos);
        }
    }
}

uint16_t World::getBlockId(sf::Vector2i pos)
{
    sf::Vector2i chunk_pos = getChunkPosFromBlockPos(pos);
    sf::Vector2i bp = getBlockPosInChunk(pos);
    if (!isChunkLoaded(chunk_pos))
        return GameBlocks::AIR->getId();
    return getChunkConst(chunk_pos).getBlockId(bp.x, bp.y);
}

uint16_t World::getGroundId(sf::Vector2i pos)
{
    sf::Vector2i chunk_pos = getChunkPosFromBlockPos(pos);
    sf::Vector2i bp = getBlockPosInChunk(pos);
    if (!isChunkLoaded(chunk_pos))
        return GameGrounds::ERROR->getId();
    return getChunkConst(chunk_pos).getGroundId(bp.x, bp.y);
}

const Block* World::getBlock(sf::Vector2i pos)
{
    return game_blocks_manager.getBlockByID(getBlockId(pos));
}

const Ground* World::getGround(sf::Vector2i pos)
{
    return game_grounds_manager.getGroundByID(getGroundId(pos));
}
