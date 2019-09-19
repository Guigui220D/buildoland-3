#pragma once

#include <map>
#include <memory>

#include "Entity.h"

class EntitiesManager
{
    public:
        EntitiesManager();
        ~EntitiesManager();

        void updateAll(float delta);
        void drawAll(sf::RenderTarget& target) const;

    private:
        std::map<
};
