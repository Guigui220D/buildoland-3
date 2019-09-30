#include "Animation.h"

Animation::Animation(size_t texture_size, size_t tileset_width, float delta) :
    texture_size(texture_size),
    tileset_width(tileset_width),
    delta(delta),
    selected_anim(0)
{

}

Animation::~Animation()
{

}

sf::IntRect Animation::getCurrentTextureRect() const
{
    int frame = (int)(anim_clock.getElapsedTime().asSeconds() / delta);
    frame %= sequences.at(selected_anim).size();

    unsigned int i = sequences.at(selected_anim).at(frame);

    return getIntRect(i);
}

void Animation::selectAnimation(unsigned int animation)
{
    if (animation == selected_anim)
        return;

    resetClock();
    if (animation < sequences.size())
        selected_anim = animation;
}

sf::IntRect Animation::getIntRect(unsigned int i) const
{
    return sf::IntRect(
        (i % tileset_width) * (texture_size + 1 * 2) + 1,
        (i / tileset_width) * (texture_size + 1 * 2) + 1,
        (texture_size + 1 * 2) - 1 * 2,
        (texture_size + 1 * 2) - 1 * 2
    );
}
