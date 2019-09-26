#pragma once

#include <SFML/Graphics.hpp>

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
class Animation
{
    public:
        Animation(float delta);
        virtual ~Animation();

        sf::IntRect getCurrentTextureRect() const;

        inline void setDelta(float new_delta) { delta = new_delta; };
        inline float getDelta() const { return delta; };

        void selectAnimation(unsigned int animation);
        inline unsigned int getCurrentAnimation() const { return selected_anim; }

        void addAnimation(std::vector<unsigned int> sequence);

        inline void resetClock() { anim_clock.restart(); }

    protected:
        float delta;
        sf::Clock anim_clock;

        unsigned int selected_anim;

        std::vector<std::vector<unsigned int>> sequences;

    private:
        sf::IntRect getIntRect(unsigned int i) const;
};



template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
Animation<TextureSize, TilesetWidth, MarginSize>::Animation(float delta) :
    delta(delta),
    selected_anim(0)
{

}

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
Animation<TextureSize, TilesetWidth, MarginSize>::~Animation()
{

}

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
sf::IntRect Animation<TextureSize, TilesetWidth, MarginSize>::getCurrentTextureRect() const
{
    int frame = (int)(anim_clock.getElapsedTime().asSeconds() / delta);
    frame %= sequences.at(selected_anim).size();

    unsigned int i = sequences.at(selected_anim).at(frame);

    return getIntRect(i);
}

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
void Animation<TextureSize, TilesetWidth, MarginSize>::selectAnimation(unsigned int animation)
{
    if (animation == selected_anim)
        return;

    resetClock();
    if (animation < sequences.size())
        selected_anim = animation;
}

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
void Animation<TextureSize, TilesetWidth, MarginSize>::addAnimation(std::vector<unsigned int> sequence)
{
    sequences.push_back(sequence);
}

template<size_t TextureSize, size_t TilesetWidth, size_t MarginSize>
sf::IntRect Animation<TextureSize, TilesetWidth, MarginSize>::getIntRect(unsigned int i) const
{
    return sf::IntRect(
        (i % TilesetWidth) * (TextureSize + MarginSize * 2) + MarginSize,
        (i / TilesetWidth) * (TextureSize + MarginSize * 2) + MarginSize,
        (TextureSize + MarginSize * 2) - MarginSize * 2,
        (TextureSize + MarginSize * 2) - MarginSize * 2
    );
}
