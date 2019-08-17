#pragma once

#include <string>

#include "BlockInfo.h"
#include "../Utils/TilesetHelper.h"
#include "../Utils/Quad.h"
#include "../Utils/Utils.h"

class Block
{
    friend class GameBlocks;
    public:
        static const TilesetHelper<16, 16, 0> tilesetHelper;

        Block(const std::string name, uint32_t default_texture);
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
         * To know whether a block is just a flat plane on the ground or a full cube
         * @param info : Info about the block
         * @return True if the block is a cube
         */
        virtual inline bool hasVolume(BlockInfo info) const { return true; }
        /**
         * To know whether the upper block should have its side rendered or not
         * Aka whether this block's textures hides everything behind it
         * @param info : Info about the block
         * @return True if the block occults the block above
         */
        virtual inline bool occults(BlockInfo info) const { return true; }
        /**
         * Get four vertices ready to add to the chunk block tops vertex array
         * @param info : Info about the block
         * @return The 4 vertices describing the texture to use
         */
        TextQuad getTopVertices(BlockInfo info) const;

    protected:
        /**
         * Get the id of the texture to use for the top of the block, in the tileset
         * @param info : Info about the block
         * @return The id to use in the tileset
         */
        virtual inline uint32_t getTopTextureNumber(BlockInfo info) const { return default_texture; }

    private:
        std::string name;
        mutable uint16_t id;

        uint32_t default_texture;
};
