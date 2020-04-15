#include "TestStructure.h"

#include "../../../common-source/Blocks/Block.h"
#include "../../../common-source/Blocks/GameBlocks.h"

#include "../../../common-source/Grounds/Ground.h"
#include "../../../common-source/Grounds/GameGrounds.h"

TestStructure::TestStructure() :
    Structure()
{
    //ctor
}

TestStructure::~TestStructure()
{
    //dtor
}

const Block* TestStructure::getBlock(sf::Vector2i block_in_chunk) const
{
    sf::Vector2i diff = pos - getRealBlockPos(chunk, block_in_chunk);

    int result = diff.x * diff.x + diff.y * diff.y;

    if (result > 81 && result <= 100)
        return GameBlocks::BOULDER;

     return GameBlocks::STRUCTURE_VOID;
}

const Ground* TestStructure::getGround(sf::Vector2i block_in_chunk) const
{
    sf::Vector2i diff = pos - getRealBlockPos(chunk, block_in_chunk);

    int result = diff.x * diff.x + diff.y * diff.y;

    if (result > 64 && result <= 121)
        return GameGrounds::DIRT;

    if (result <= 36)
        return GameGrounds::STONE;

    return GameGrounds::STRUCTURE_VOID;
}



