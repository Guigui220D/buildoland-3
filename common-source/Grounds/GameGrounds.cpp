#include "GameGrounds.h"

#include <limits>

#include "Ground.h"

#include "StaticGround.h"
#include "GameGrounds/GroundError.h"
#include "GameGrounds/GroundDirt.h"
#include "GameGrounds/GroundStone.h"
#include "GameGrounds/GroundSand.h"
#include "GameGrounds/GroundGrass.h"
#include "GameGrounds/GroundWater.h"
#include "GameGrounds/GroundCarpet.h"
#include "GameGrounds/GroundParquet.h"

#include "../../common-source/Utils/Log.h"

Ground const * const GameGrounds::ERROR =           new GroundError("error");
Ground const * const GameGrounds::STRUCTURE_VOID =  new GroundError("structural_void");

Ground const * const GameGrounds::DIRT =            new GroundDirt();
Ground const * const GameGrounds::WATER =           new GroundWater();
Ground const * const GameGrounds::STONE =           new GroundStone();
Ground const * const GameGrounds::SAND  =           new GroundSand();
Ground const * const GameGrounds::GRASS =           new GroundGrass();
Ground const * const GameGrounds::WOOD =            new GroundParquet();
Ground const * const GameGrounds::CARPET =          new GroundCarpet();


GameGrounds::GameGrounds()
{
    //ctor
}

GameGrounds::~GameGrounds()
{
    //dtor
}

void GameGrounds::initGrounds()
{
    names.clear();
    grounds.clear();
    //Add all grounds here
    addGround(ERROR);
    addGround(STRUCTURE_VOID);

    addGround(WATER);
    addGround(DIRT);
    addGround(STONE);
    addGround(SAND);
    addGround(GRASS);
    addGround(WOOD);
    addGround(CARPET);
}

void GameGrounds::addGround(Ground const * ground)
{
    std::string name = ground->getName();
    //Fail if the name is already used
    assert(names.find(name) == names.end());
    //Fail if theres more grounds than the id system can allow (very unlikely)
    assert(grounds.size() < std::numeric_limits<uint16_t>::max() - 1);
    uint16_t id = (uint16_t)grounds.size();

    names.emplace(name, id);
    grounds.push_back(ground);
    ground->id = id;

    log(INFO, "Added ground \"{}\" with id {}\n", name, id);
}

Ground const * GameGrounds::getGroundByID(uint16_t id) const
{
    if (id >= grounds.size())
        return ERROR;

    return grounds.at(id);
}

Ground const * GameGrounds::getGroundByName(std::string name) const
{
    auto ptr = names.find(name);

    if (ptr == names.end())
        return ERROR;

    uint16_t id = ptr->second;
    assert(id < grounds.size());

    return grounds.at(id);
}
