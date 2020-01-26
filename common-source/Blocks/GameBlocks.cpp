#include "GameBlocks.h"

#include <limits>
#include <iostream>

Block const * const GameBlocks::AIR             = new BlockAir();
Block const * const GameBlocks::ERROR           = new BlockError();
Block const * const GameBlocks::STONE           = new Block("stone", true, 2);
Block const * const GameBlocks::STONE_BRICKS    = new Block("bricks", true, 3);
Block const * const GameBlocks::IRON            = new Block("iron", true, 4);
Block const * const GameBlocks::GOLD            = new Block("gold", true, 5);
Block const * const GameBlocks::WOOD            = new Block("wood", true, 7);
Block const * const GameBlocks::BOULDER         = new BlockBoulder();
Block const * const GameBlocks::DOOR_CLOSED     = new BlockDoorClosed();

GameBlocks::GameBlocks()
{
    //ctor
}

GameBlocks::~GameBlocks()
{
    //dtor
}

void GameBlocks::initBlocks()
{
    names.clear();
    blocks.clear();
    //Add all blocks here
    addBlock(AIR);
    addBlock(ERROR);

    addBlock(STONE);
    addBlock(STONE_BRICKS);
    addBlock(IRON);
    addBlock(GOLD);
    addBlock(WOOD);
    addBlock(BOULDER);
    addBlock(DOOR_CLOSED);
}

void GameBlocks::addBlock(Block const * block)
{
    std::string name = block->getName();
    //Fail if the name is already used
    assert(names.find(name) == names.end());
    //Fail if theres more blocks than the id system can allow (very unlikely)
    assert(blocks.size() < std::numeric_limits<uint16_t>::max() - 1);
    uint16_t id = (uint16_t)blocks.size();

    names.emplace(name, id);
    blocks.push_back(block);
    block->id = id;

    std::cout << "Added block \"" << name << "\" with id " << id << std::endl;
}

Block const * GameBlocks::getBlockByID(uint16_t id) const
{
    if (id >= blocks.size())
        return ERROR;

    return blocks.at(id);
}

Block const * GameBlocks::getBlockByName(const std::string name) const
{
    auto ptr = names.find(name);

    if (ptr != names.end())
        return ERROR;

    uint16_t id = ptr->second;
    assert(id < blocks.size());

    return blocks.at(id);
}
