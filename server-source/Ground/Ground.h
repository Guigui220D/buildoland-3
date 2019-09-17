#pragma once

#include <string>
#include <SFML/System.hpp>

#include "GroundInfo.h"

class Ground
{
    friend class GameGrounds;

    public:
        Ground(const std::string name);
        virtual ~Ground();

        /**
         * Get the name of this ground
         * @return The name of this ground
         */
        inline const std::string getName() const { return name; }
        /**
         * Get the id of this ground
         * The id is "calculated" when the ground is added to the ground manager
         * @return The id of this ground
         */
        inline uint16_t getId() const { return id; }

    protected:

    private:
        std::string name;
        mutable uint16_t id;
};
