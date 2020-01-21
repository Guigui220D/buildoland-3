#pragma once

#include "GuiElement.h"

class GuiTitle : public GuiElement
{
    public:
        GuiTitle(Game& game);
        ~GuiTitle();

        void draw(sf::RenderTarget& target) const override;
        void init() override;

        inline void setFade(uint8_t fade) { title_sprite.setFillColor(sf::Color(255, 255, 255, fade)); }

    private:
        sf::Texture title_texture;
        sf::RectangleShape title_sprite;
};
