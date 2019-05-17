#pragma once

#include <SFML/Graphics.hpp>

#include "GuiElement.h"

class GuiLoadingIcon : public GuiElement
{
    public:
        GuiLoadingIcon(sf::Vector2u window_size);
        virtual ~GuiLoadingIcon();

        virtual void draw(sf::RenderTarget& target) const override;
        virtual void update(float delta_time) override;

    protected:
        sf::RectangleShape icon;

    private:
};
