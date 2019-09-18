#pragma once

#include "GuiElement.h"

class GuiButton : public GuiElement
{
    public:
        GuiButton(Game* game, sf::FloatRect zone, float aspect_ratio, GuiAlign horizontal_align, GuiAlign vertical_align);
        virtual ~GuiButton();

        virtual bool handleEvent(sf::Event& event) override;
        virtual void draw(sf::RenderTarget& target) const override;
        //virtual void update(float delta_time) override;

        /**
        * To check if this button has been clicked since the last call of this function
        * @return True if it has been clicked
        */
        inline bool hasBeenClicked()
        {
            if (clicked)
            {
                clicked = false;
                return true;
            }
            return false;
        }

    protected:
        sf::RectangleShape button;

        bool isMouseInside() const;

    private:
        bool waiting_for_release = false;
        bool clicked = false;
};
