#include "NaturalGenerator.h"

#include "../../../common-source/Grounds/GameGrounds.h"
#include "../../../common-source/Blocks/GameBlocks.h"

#include "../../../external/xxhash/XXHash.hpp"
#include "../../../external/xxhash/XXHash_bis.hpp"

#include "../Chunk.h"

NaturalGenerator::NaturalGenerator(int seed) :
    Generator(seed),
    perlin((uint32_t)seed)
{
}

NaturalGenerator::~NaturalGenerator()
{
    //dtor
}

void NaturalGenerator::init(const GameBlocks& game_blocks, const GameGrounds& game_grounds)
{
    test_struct_2.load("Server-Resources/Structures/test_structure_2.json", game_blocks, game_grounds);

    ground_levels.push_back(GameGrounds::WATER);
    ground_levels.push_back(GameGrounds::WATER);
    ground_levels.push_back(GameGrounds::SAND);
    ground_levels.push_back(GameGrounds::GRASS);
    ground_levels.push_back(GameGrounds::GRASS);
    ground_levels.push_back(GameGrounds::STONE);
    ground_levels.push_back(GameGrounds::STONE);

    random_blocks.push_back(GameBlocks::IRON);
    random_blocks.push_back(GameBlocks::GOLD);
    random_blocks.push_back(GameBlocks::STONE);
    random_blocks.push_back(GameBlocks::STONE_BRICKS);
    random_blocks.push_back(GameBlocks::WOOD);
}

void NaturalGenerator::generateChunk(Chunk* chunk)
{
    test_struct_2.setChunk(chunk->getPos());

    if (std::rand() % 10 == 0)
    {
        for (int x = 0; x < Chunk::CHUNK_SIZE; x++)
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++)
        {
            chunk->setGround(x, y, GameGrounds::WOOD);

            if (std::rand() & 1)
                chunk->setBlock(x, y, random_blocks.at(std::rand() % random_blocks.size()));
        }
    }
    // carpet version ?
    else if (std::rand() % 10 == 0)
    {
        for (int x = 0; x < Chunk::CHUNK_SIZE; x++)
            for (int y = 0; y < Chunk::CHUNK_SIZE; y++)
            {
                chunk->setGround(x, y, GameGrounds::CARPET);

                if (std::rand() & 1)
                    chunk->setBlock(x, y, random_blocks.at(std::rand() % random_blocks.size()));
            }
    }
    else
    {
        for (int x = 0; x < Chunk::CHUNK_SIZE; x++)
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++)
        {
            sf::Vector2i g_pos = chunk->getPos() * Chunk::CHUNK_SIZE + sf::Vector2i(x, y);
            double perlin_value = perlin.noise0_1(((double)g_pos.x) / Chunk::CHUNK_SIZE, ((double)g_pos.y) / Chunk::CHUNK_SIZE) * (ground_levels.size()) - 1.;

            if (perlin_value < 0.)
                perlin_value = 0.;

            if (std::round(perlin_value) >= 5 && std::rand() % 22 == 0)
                chunk->setBlock(x, y, GameBlocks::STONE_PLATE);

            if (std::round(perlin_value) >= 3 && std::round(perlin_value) <= 4 && std::rand() % 18 == 0)
                chunk->setBlock(x, y, GameBlocks::STICKS);

            if (std::round(perlin_value) >= 6 && std::rand() % 3 == 0)
            {
                switch (std::rand() % 4)
                {
                case 0: case 1:
                    chunk->setBlock(x, y, GameBlocks::BOULDER);
                    break;
                case 2:
                    chunk->setBlock(x, y, GameBlocks::COPPER_ORE);
                    break;
                case 3:
                    chunk->setBlock(x, y, GameBlocks::GOLD_ORE);
                    break;
                }
            }

            if (std::round(perlin_value) >= 3 && std::round(perlin_value) <= 4 && std::rand() % 40 == 0)
                chunk->setBlock(x, y, GameBlocks::TREE);

            chunk->setGround(x, y, ground_levels.at(std::round(perlin_value)));
        }
    }

    for (int x = 0; x < Chunk::CHUNK_SIZE; x++)
    for (int y = 0; y < Chunk::CHUNK_SIZE; y++)
    {
        const Block* structure_block = test_struct_2.getBlock(sf::Vector2i(x, y));
        if (structure_block != GameBlocks::STRUCTURE_VOID)
            chunk->setBlock(x, y, structure_block);

        const Ground* structure_ground = test_struct_2.getGround(sf::Vector2i(x, y));
        if (structure_ground != GameGrounds::STRUCTURE_VOID)
            chunk->setGround(x, y, structure_ground);
    }

    if (chunk->getPos() == sf::Vector2i())
    {
        chunk->setBlock(0, 0, GameBlocks::AIR);
        chunk->setBlock(15, 15, GameBlocks::CHEST);
    }
}

/*
bool NaturalGenerator::hasStructure(sf::Vector2i chunk_pos) const
{
    return (XXH32(&chunk_pos, sizeof(chunk_pos), getSeed()) & 0b111) == 0; //1 / 8 chance
}

sf::Vector2i NaturalGenerator::getStructurePos(sf::Vector2i chunk_pos) const
{
    unsigned int x = XXH32(&chunk_pos, sizeof(chunk_pos), getSeed() + 1) % Chunk::CHUNK_SIZE;
    unsigned int y = XXH32(&chunk_pos, sizeof(chunk_pos), getSeed() + 2) % Chunk::CHUNK_SIZE;
    return sf::Vector2i(x, y);
}
*/
