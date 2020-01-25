#pragma once

#include <string>

#include "BlockInfo.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/Utils/TilesetHelper.h"
    #include "../../client-source/Utils/Quad.h"
    #include "../../client-source/Utils/Utils.h"
#else
    #include "../Items/ItemStack.h"
    #include <vector>
#endif

class Item;

class Block
{
    friend class GameBlocks;
    friend class ItemsRegister;
    friend class BlockItem;
    public:
        #ifdef CLIENT_SIDE
        static const TilesetHelper<16, 16, 1> tilesetHelper;
        #endif

        Block(const std::string name, bool should_have_item, uint32_t default_texture);    //Default texture shall be ignored in server

        virtual ~Block();

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

        /**
         * To know whether an entity's hitbox can touch that box
         * @param info : Info about the block
         * @return True if the block can be touched
         */
        virtual inline bool isSolid(BlockInfo info) const { return true; }
        /**
         * To know whether a block is just a flat plane on the ground or a full cube
         * @param info : Info about the block
         * @return True if the block is a cube
         */
        virtual inline bool hasVolume(BlockInfo info) const { return true; }

        #ifdef CLIENT_SIDE
        /**
         * To know whether the upper block should have its side rendered or not
         * Aka whether this block's textures hides everything behind it
         * @param info : Info about the block
         * @return True if the block occults the block above
         */
        virtual inline bool occults(BlockInfo info) const { return true; }
        /**
         * To know whether the block side should be rendered even if the block below it occults
         * @return True if the block side is always visible
         */
        virtual inline bool alwaysVisible() const { return false; }
        /**
         * Get four vertices ready to add to the chunk block tops vertex array
         * @param info : Info about the block
         * @return The 4 vertices describing the texture to use
         */
        virtual TextQuad getTopVertices(BlockInfo info) const;
        /**
         * Get four vertices ready to add to the chunk block sides vertex array
         * @param info : Info about the block
         * @return The 4 vertices describing the texture to use
         */
        virtual TextQuad getSideVertices(BlockInfo info) const;
        #else
        virtual std::vector<ItemStack> getDrops() const;

        inline Item const * getDefaultItem() const { return drop; };
        #endif

    protected:
        #ifdef CLIENT_SIDE
        /**
         * Get the id of the texture to use for the top of the block, in the tileset
         * @param info : Info about the block
         * @return The id to use in the tileset
         */
        virtual inline uint32_t getTopTextureNumber(BlockInfo info) const { return default_texture; }
        /**
         * Get the id of the texture to use for the side of the block, in the tileset
         * @param info : Info about the block
         * @return The id to use in the tileset
         */
        virtual inline uint32_t getSideTextureNumber(BlockInfo info) const { return default_texture; }
        /**
         * To know whether the side should be darkened or not
         * @return True if the side should be darkened
         */
        virtual inline bool darkenSide() const { return true; }
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
