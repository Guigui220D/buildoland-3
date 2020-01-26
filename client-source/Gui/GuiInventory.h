#pragma once
#include "GuiElement.h"

#include "../../common-source/Items/PlayerInventory.h"

class GuiInventory : public GuiElement
{
    public:
        GuiInventory(Game& game, PlayerInventory& inv);
        ~GuiInventory();

        bool handleEvent(sf::Event& event) override;
        void draw(sf::RenderTarget& target) const override;
        void init() override;
        void update(float delta_time) override;

    private:
        sf::Texture const * texture;
        sf::RectangleShape rectangle;

        mutable sf::Text hover_text;

        mutable sf::RectangleShape item_hand;

        mutable sf::RectangleShape item_draw;
        mutable sf::Text number_draw;

        sf::Texture const * block_textures;
        sf::Texture const * ground_textures;
        sf::Texture const * item_textures;

        PlayerInventory& inventory;
};
