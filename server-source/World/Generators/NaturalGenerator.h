#pragma once

#include "../Generator.h"

#include "../../../external/perlin/PerlinNoise.hpp"
#include <vector>
#include <SFML/System.hpp>

#include "../Structures/TestStructure.h"
#include "../Structures/StructureFromJson.h"

class Ground;
class Block;

class GameBlocks;
class GameGrounds;

class NaturalGenerator : public Generator
{
    public:
        NaturalGenerator(int seed);
        ~NaturalGenerator();

        void init(const GameBlocks& game_blocks, const GameGrounds& game_grounds) override;

        void generateChunk(Chunk* chunk, std::vector<Entity*>& new_entities) override;

    private:
        siv::PerlinNoise perlin;

        std::vector<Ground const *> ground_levels;
        std::vector<Block const *> random_blocks;

        //bool hasStructure(sf::Vector2i chunk_pos) const;
        //sf::Vector2i getStructurePos(sf::Vector2i chunk_pos) const;

        uint32_t getRandomInt(sf::Vector2i pos, int modifier = 0);

        TestStructure test_struct;
        StructureFromJson test_struct_2;

};
