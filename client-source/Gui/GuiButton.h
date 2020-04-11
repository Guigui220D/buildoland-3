#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "GuiElement.h"

class GuiButton : public GuiElement
{
    public:
        GuiButton(Game& game, sf::Vector2f position, float width, const sf::String& text_string);
        virtual ~GuiButton();

        virtual bool handleEvent(sf::Event& event) override;
        virtual void draw(sf::RenderTarget& target) const override;
        virtual void init() override;
        virtual void update(float delta_time) override;

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
        sf::Text text;

        sf::String string;

        bool isMouseInside() const;

    private:
        bool waiting_for_release = false;
        bool clicked = false;
};
