#pragma once

#include "Block.h"

#include "GameBlocks/BlockAir.h"
#include "GameBlocks/BlockStone.h"
#include "GameBlocks/BlockStoneBricks.h"
#include "GameBlocks/BlockIron.h"
#include "GameBlocks/BlockGold.h"
#include "GameBlocks/BlockTree.h"

#include <vector>
#include <map>
#include <string>
#include <assert.h>

class GameBlocks
{
    public:
        GameBlocks();
        ~GameBlocks();

        /**
         * Inits the block list (reinits if called again)
         */
        void initBlocks();

        /**
         * Gets a pointer to a block with its ID
         * @param id : the id of the block you want
         * @return The pointer to the block
         */
        Block const * getBlockByID(uint16_t id) const;
        /**
         * Gets a pointer to a block using its name
         * @param name : the name of the block
         * @return The pointer to the block
         */
        Block const * getBlockByName(const std::string name) const;

        //Put blocks here
        static BlockAir         const * const AIR;
        static BlockStone       const * const STONE;
        static BlockStoneBricks const * const STONE_BRICKS;
        static BlockIron        const * const IRON;
        static BlockGold        const * const GOLD;
        static BlockTree        const * const TREE;

    private:
        std::vector<Block const *> blocks;
        std::map<std::string, uint16_t> names;

        /**
         * Adds a block to the block list and its name
         * Fails if theres more blocks than the id type allows or if the name is already used
         * @param block : the block to add, must be a constant pointer to a constant block inside of THIS class
         */
        void addBlock(Block const * block);
};
