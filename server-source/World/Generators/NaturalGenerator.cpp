#include "NaturalGenerator.h"

NaturalGenerator::NaturalGenerator(int seed) :
    Generator(seed),
    perlin((uint32_t)seed)
{
}

NaturalGenerator::~NaturalGenerator()
{
    //dtor
}

void NaturalGenerator::init()
{
    ground_levels.push_back(GameGrounds::WATER->getId());
    ground_levels.push_back(GameGrounds::WATER->getId());
    ground_levels.push_back(GameGrounds::SAND->getId());
    ground_levels.push_back(GameGrounds::GRASS->getId());
    ground_levels.push_back(GameGrounds::GRASS->getId());
    ground_levels.push_back(GameGrounds::STONE->getId());
    ground_levels.push_back(GameGrounds::STONE->getId());

    random_blocks.push_back(GameBlocks::IRON->getId());
    random_blocks.push_back(GameBlocks::GOLD->getId());
    random_blocks.push_back(GameBlocks::STONE->getId());
    random_blocks.push_back(GameBlocks::STONE_BRICKS->getId());
    random_blocks.push_back(GameBlocks::WOOD->getId());
}

void NaturalGenerator::generateChunk(Chunk* chunk)
{
    if (std::rand() % 10 == 0 && chunk->getPos() != sf::Vector2i())
    {
        for (int x = 0; x < Chunk::CHUNK_SIZE; x++)
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++)
        {
            chunk->setGround(x, y, GameGrounds::WOOD);

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

            if (std::round(perlin_value) >= 4 && std::rand() % 20 == 0)
                chunk->setBlock(x, y, GameBlocks::BOULDER);

            chunk->setGround(x, y, ground_levels.at(std::round(perlin_value)));
        }
    }
}
