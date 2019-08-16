#pragma once

#include "State.h"

#include "../World/World.h"

class GameState : public State
{
    public:
        GameState(Game* game, unsigned int id = 0);
        ~GameState();

        bool handleEvent(sf::Event& event) override;
        void update(float delta_time) override;
        void draw(sf::RenderTarget& target) const override;

    private:
        World test_world;

        sf::View my_view;

        void updateView();

        sf::Clock anim_clock;
        int anim_frame = 0;
};
