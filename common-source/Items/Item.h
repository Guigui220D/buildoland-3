#pragma once

#include <string>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#ifdef CLIENT_SIDE
    #include "../../client-source/Utils/TilesetHelper.h"
#endif // CLIENT_SIDE

#include "../Networking/NetworkingCodes.h"

class World;
class ItemStack;
class Player;

class Item
{
    friend class ItemsRegister;
    public:
        #ifdef CLIENT_SIDE
        static const TilesetHelper<16, 16, 1> tilesetHelper;
        #endif

        enum TextureSet
        {
            ItemsTextureSet = 0,
            GroundsTextureSet,
            BlocksTextureSet,

            TextureSetEnd
        };


        Item(const std::string name, uint32_t default_texture); //Default texture ignored by server
        virtual ~Item();

        /**
         * Get the name of this item
         * @return The name of this item
         */
        inline const std::string getName() const { return name; }
        /**
         * Get the id of this item
         * The id is determined when the item is added to the item manager
         * @return The id of this item
         */
        inline uint16_t getId() const { return id; }

        /**
         * Use the item
         * @param stack : a reference to the item stack this has been used from
         * @param world : a reference to the world, in case it has to be modified
         * @param click_pos : the position that has been clicked in the world
         */
        void useItem(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

        #ifdef CLIENT_SIDE
        virtual inline sf::IntRect getTexture(ItemStack& stack) const { return tilesetHelper.getIntRect(default_texture); }
        virtual inline TextureSet getTexturesSet() const { return TextureSet::ItemsTextureSet; }

        inline std::string& getDisplayName() const { return display_name; }
        inline void addTexturedVertices(ItemStack& stack, sf::Vertex vertices[4]) const
        {
            sf::IntRect tex_rect = getTexture(stack);
            vertices[0].texCoords = sf::Vector2f(tex_rect.left, tex_rect.top);
            vertices[1].texCoords = sf::Vector2f(tex_rect.left+tex_rect.width, tex_rect.top);
            vertices[2].texCoords = sf::Vector2f(tex_rect.left+tex_rect.width, tex_rect.top+tex_rect.height);
            vertices[3].texCoords = sf::Vector2f(tex_rect.left, tex_rect.top+tex_rect.height);
        }
        #endif // CLIENT_SIDE

    protected:
        /**
         * Use the item
         * Called by useItem, to be overriden
         */
        virtual void use(ItemStack& stack, World& world, sf::Vector2i click_pos, Player& player) const;

    private:
        std::string name;
        mutable uint16_t id = 0;

        #ifdef CLIENT_SIDE
        uint32_t default_texture;

        mutable std::string display_name;
        #endif // CLIENT_SIDE
};
