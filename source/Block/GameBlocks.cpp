#include "GameBlocks.h"

#include <limits>
#include <iostream>

BlockAir    const * const GameBlocks::AIR   = new BlockAir();
BlockStone  const * const GameBlocks::STONE = new BlockStone();


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
    addBlock(STONE);
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

    std::cout << "Added block \"" << name << "\" with id " << id << std::endl;
}

Block const * GameBlocks::getBlockByID(unsigned short id) const
{
    if (id >= blocks.size())
        return AIR;

    return blocks.at(id);
}

Block const * GameBlocks::getBlockByName(const std::string name) const
{
    auto ptr = names.find(name);

    if (ptr != names.end())
        return AIR;

    uint16_t id = ptr->second;
    assert(id < blocks.size());

    return blocks.at(id);
}
