#pragma once

#include "GuiElement.h"

class GuiImage : public GuiElement
{
    public:
        GuiImage(Game& game, sf::FloatRect zone, float aspect_ratio, GuiAlign horizontal_align, GuiAlign vertical_align, const std::string texture_path, bool use_resource_manager = true);
        ~GuiImage();

        void draw(sf::RenderTarget& target) const override;
        void init() override;

        inline void setTransparency(uint8_t fade) { rectangle.setFillColor(sf::Color(255, 255, 255, fade)); }

    private:
        sf::Texture* texture;
        sf::RectangleShape rectangle;

        const std::string texture_path;
        const bool use_manager;
};
