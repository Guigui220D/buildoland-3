#include "GameGrounds.h"

#include <limits>
#include <iostream>

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
    addGround(STONE);
    addGround(SAND);
    addGround(GRASS);
    addGround(WATER);
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

    std::cout << "Added ground \"" << name << "\" with id " << id << std::endl;
}

Ground const * GameGrounds::getGroundByID(uint16_t id) const
{
    if (id >= grounds.size())
        return STONE;

    return grounds.at(id);
}

Ground const * GameGrounds::getGroundByName(const std::string name) const
{
    auto ptr = names.find(name);

    if (ptr != names.end())
        return STONE;

    uint16_t id = ptr->second;
    assert(id < grounds.size());

    return grounds.at(id);
}
