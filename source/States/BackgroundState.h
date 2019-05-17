#pragma once

#include <SFML/Graphics.hpp>

#include "State.h"

//This state should always be underneath ever other state and blocks everything
class BackgroundState : public State
{
    public:
        BackgroundState(Game* game, unsigned int id = 0);
        virtual ~BackgroundState();

        virtual void init() override;
        virtual bool handleEvent(sf::Event& event) override;
        virtual void draw(sf::RenderTarget& target) const override;

    private:
        sf::Texture background_texture;
        sf::RectangleShape background_rectangle;
        sf::View view;
};
