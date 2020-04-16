#pragma once

#include <string>
#include <vector>

#include "../Structure.h"

class Block;
class Ground;

class GameBlocks;
class GameGrounds;

class StructureFromJson : public Structure
{
    public:
        StructureFromJson();
        ~StructureFromJson();

        void load(std::string json_path, const GameBlocks& game_blocks, const GameGrounds& game_grounds);

        const Block* getBlock(sf::Vector2i block_in_chunk) const override;
        const Ground* getGround(sf::Vector2i block_in_chunk) const override;

    private:
        size_t size_x, size_y;
        int origin_x, origin_y;

        std::vector<const Block*> blocks;
        std::vector<const Ground*> grounds;
};
