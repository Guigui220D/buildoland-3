#pragma once

#include "../Generator.h"

#include "../../../external/perlin/PerlinNoise.hpp"
#include <vector>

class Ground;
class Block;

class NaturalGenerator : public Generator
{
    public:
        NaturalGenerator(int seed);
        ~NaturalGenerator();

        void init() override;

        void generateChunk(Chunk* chunk) override;

    private:
        siv::PerlinNoise perlin;

        std::vector<Ground const *> ground_levels;
        std::vector<Block const *> random_blocks;
};
