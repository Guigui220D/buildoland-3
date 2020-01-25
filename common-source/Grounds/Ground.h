#pragma once

#include <string>
#include <SFML/System.hpp>

#ifdef CLIENT_SIDE
    #include "../../client-source/Utils/TilesetHelper.h"
    #include "../../client-source/Utils/Utils.h"
#else
    #include "../Items/ItemStack.h"
    #include <vector>
#endif // CLIENT_SIDE

#include "GroundInfo.h"

class Ground
{
    friend class GameGrounds;
    friend class GroundItem;
    friend class ItemsRegister;
    public:
        #ifdef CLIENT_SIDE
        static const TilesetHelper<16, 16, 1> tilesetHelper;
        static const TilesetHelper<16, 8, 1> tilesetHelperDetails;
        #endif

        Ground(const std::string name, bool should_have_item, uint32_t default_texture);   //Textude id shall be ignored in server
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
        /**
         * @return Returns true if that ground has an item
         */
        inline bool hasItem() const { return has_item; }

        #ifdef CLIENT_SIDE
        /**
         * Get four vertices ready to add to the chunk vertex array, for the base texture
         * @param info : Info about the ground
         * @return The 4 vertices describing the texture to use
         */
        inline Quad getTextureVertices(GroundInfo info) const { return tilesetHelper.getFourVertices(getTextureNumber(info), getTextureRotation(info)); }

        /**
         * Get wether or not that ground has surface details
         * @param info : Info about the ground
         * @return True if the ground has surface details
         */
        virtual inline bool hasSurfaceDetails(GroundInfo info) const { return true; }
        /**
         * Calculate and get this ground detail's vertices (including other block's bleedings)
         * @param info : Info about the ground
         * @param frame : The frame of the animation (0, 1, 2, 3) to make changing vertex arrays
         * @return A vertex array to add to the chunk vertex array
         */
        virtual sf::VertexArray getSurfaceDetails(GroundInfo info, int frame) const;
        #else
        virtual std::vector<ItemStack> getDrops() const;

        inline Item const * getDefaultItem() const { return drop; };
        #endif
    protected:
        #ifdef CLIENT_SIDE
        /**
         * Get the id of the texture to use, in the tileset
         * @param info : Info about the ground
         * @return The id to use in the tileset
         */
        virtual inline uint32_t getTextureNumber(GroundInfo info) const { return default_texture; }
        /**
         * Get the rotation of the texture, often always 0 or a random value for natural grounds*
         * Should be 0, 1, 2 or 3
         * @param info : Info about the ground
         * @return The rotation
         */
        virtual inline uint8_t getTextureRotation(GroundInfo info) const { return getRandomInt(info) % 4; }

        /**
         * To know if this texture makes bleedings for other ground's details
         * @return True if this texture "bleeds"
         */
        virtual inline bool hasTextureBleedings() const { return false; }
        /**
         * Get the id of the texture to use for bleeding in the tileset helper
         * @param info : Info about the ground
         * @return The id of the texture to use
         */
        virtual inline uint32_t getBleedingForNeighborGrounds(GroundInfo info, int frame) const { return 0; }

        /**
         * To know if this ground will get the bleedings of an other specific ground
         * @param info : Info about the ground
         * @param other : The other ground
         * @return True if the other ground's bleedings should be displayed
         */
        virtual bool acceptsTextureBleedings(GroundInfo info, const Ground* other) const;
        /**
         * Add the neighbor bleedings to a vertex array (for use in getSurfaceDetails)
         * @param info : Info about the ground
         * @param vertex_array : A reference to a vertex array to add the vertices on
         */
        void addNeighborsBleeding(GroundInfo info, sf::VertexArray& vertex_array, int frame) const;

        /**
         * Get a random-looking int deterministically depending on the position of the ground and seed of the world
         * @param info : Info about the ground, for position and seed
         * @return The pseudo-random int
         */
        static uint32_t getRandomInt(GroundInfo info, int add = 0);
        #endif

    private:
        std::string name;
        mutable uint16_t id = 0;

        const bool has_item;

        #ifdef CLIENT_SIDE
        uint32_t default_texture;
        #else
        mutable Item const * drop;
        #endif
};
