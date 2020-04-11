#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Clock.hpp>

#include <vector>

class Animation
{
    public:
        Animation(size_t texture_size, size_t tileset_width, float delta);
        virtual ~Animation();

        sf::IntRect getCurrentTextureRect() const;

        inline void setDelta(float new_delta) { delta = new_delta; };
        inline float getDelta() const { return delta; };

        inline void setTextureSize(size_t new_texture_size) { texture_size = new_texture_size; };
        inline size_t getTextureSize() const { return texture_size; };

        inline void setTilesetWidth(size_t new_tileset_width) { tileset_width = new_tileset_width; };
        inline size_t getTilesetWidth() const { return tileset_width; };

        void selectAnimation(unsigned int animation);
        inline unsigned int getCurrentAnimation() const { return selected_anim; }

        inline void addAnimation(std::vector<unsigned int> sequence) { sequences.push_back(sequence); }

        inline void resetClock() { anim_clock.restart(); }

    protected:
        size_t texture_size;
        size_t tileset_width;

        float delta;
        sf::Clock anim_clock;

        unsigned int selected_anim;

        std::vector<std::vector<unsigned int>> sequences;

    private:
        sf::IntRect getIntRect(unsigned int i) const;
};
