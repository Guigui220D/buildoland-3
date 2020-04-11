#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "GuiElement.h"

namespace sf
{
class Texture;
}

class GuiImage : public GuiElement
{
    public:
        GuiImage(Game& game, sf::Vector2f position, sf::Vector2f size, const std::string& texture_path, bool use_resource_manager = true, bool smooth = false);
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
