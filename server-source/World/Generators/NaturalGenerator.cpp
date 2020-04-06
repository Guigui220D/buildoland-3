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
    if (chunk->getPos() == sf::Vector2i())
        chunk->setBlock(0, 0, GameBlocks::AIR);
}
