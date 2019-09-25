#pragma once

#include <vector>
#include <memory>

#include "GuiElement.h"

class GuiManager : public std::vector<GuiElement*>
{
    public:
        GuiManager();
        virtual ~GuiManager();

        void updateWindowSize(sf::Vector2u window_size);
        void initEverything();
        bool handleEvent(sf::Event& event);
        void updateEverything(float delta_time);
        void drawEverything(sf::RenderTarget& target) const;

    protected:

    private:
};
