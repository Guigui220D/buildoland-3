#pragma once

#include <vector>
#include <memory>

#include "GuiZone.h"

class GuiManager
{
    public:
        GuiManager();
        ~GuiManager();

        void updateWindowSize(sf::Vector2u window_size);
        void initEverything();
        bool handleEvent(sf::Event& event);
        void updateEverything(float delta_time);
        void drawEverything(sf::RenderTarget& target) const;

    private:
        std::vector<GuiZone> zones;
};
