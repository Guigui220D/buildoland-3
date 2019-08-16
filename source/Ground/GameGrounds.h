#pragma once

#include "Ground.h"

#include "GameGrounds/GroundStone.h"
#include "GameGrounds/GroundSand.h"
#include "GameGrounds/GroundGrass.h"
#include "GameGrounds/GroundWater.h"

#include <vector>
#include <map>
#include <string>
#include <assert.h>

class GameGrounds
{
    public:
        GameGrounds();
        ~GameGrounds();

        /**
         * Inits the ground list (reinits if called again)
         */
        void initGrounds();

        /**
         * Gets a pointer to a ground with its ID
         * @param id : the id of the ground you want
         * @return The pointer to the ground
         */
        Ground const * getGroundByID(uint16_t id) const;
        /**
         * Gets a pointer to a ground using its name
         * @param name : the name of the ground
         * @return The pointer to the ground
         */
        Ground const * getGroundByName(const std::string name) const;

        //Put grounds here
        GroundWater const * const WATER = new GroundWater();
        GroundStone const * const STONE = new GroundStone();
        GroundSand const * const SAND = new GroundSand();
        GroundGrass const * const GRASS = new GroundGrass();

    private:
        std::vector<Ground const *> grounds;
        std::map<std::string, uint16_t> names;

        /**
         * Adds a ground to the ground list and its name
         * Fails if theres more grounds than the id type allows or if the name is already used
         * @param ground : the ground to add, must be a constant pointer to a constant ground inside of THIS class
         */
        void addGround(Ground const * ground);
};
