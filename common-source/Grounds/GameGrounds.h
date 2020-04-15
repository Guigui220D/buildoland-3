#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <assert.h>

class Ground;

class GameGrounds
{
    friend class ItemsRegister;
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
        static Ground const * const ERROR;
        static Ground const * const STRUCTURE_VOID;

        static Ground const * const DIRT;
        static Ground const * const WATER;
        static Ground const * const STONE;
        static Ground const * const SAND;
        static Ground const * const GRASS;
        static Ground const * const CARPET;
        static Ground const * const WOOD;

    private:
        std::vector<Ground const *> grounds;
        std::unordered_map<std::string, uint16_t> names;

        /**
         * Adds a ground to the ground list and its name
         * Fails if theres more grounds than the id type allows or if the name is already used
         * @param ground : the ground to add, must be a constant pointer to a constant ground inside of THIS class
         */
        void addGround(Ground const * ground);
};
