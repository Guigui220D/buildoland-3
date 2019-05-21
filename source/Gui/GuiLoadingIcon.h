#pragma once

#include "GuiElement.h"

class GuiLoadingIcon : public GuiElement
{
    public:
        GuiLoadingIcon(Game* game);
        virtual ~GuiLoadingIcon();

        virtual void draw(sf::RenderTarget& target) const override;
        virtual void update(float delta_time) override;

    protected:
        sf::RectangleShape icon;

    private:
};
