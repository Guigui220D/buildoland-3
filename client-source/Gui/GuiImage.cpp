#include "GuiImage.h"

#include "../Game.h"

GuiImage::GuiImage(Game& game, sf::Vector2f position, sf::Vector2f size, const std::string texture_path, bool use_resource_manager, bool smooth) :
    GuiElement(game),
    texture_path(texture_path),
    use_manager(use_resource_manager)
{
    rectangle.setPosition(position);
    rectangle.setSize(size);

    if (!use_resource_manager)
    {
        texture = new sf::Texture();
        texture->setSmooth(smooth);
    }
}

GuiImage::~GuiImage()
{
    if (!use_manager)
        delete texture;
}

void GuiImage::init()
{
    if (use_manager)
        texture = const_cast<sf::Texture*>(&getGame().getResourceManager().getTexture(texture_path));
    else
        texture->loadFromFile(texture_path);

    rectangle.setTexture(texture);
    //rectangle.setSize(getSize());
}

void GuiImage::draw(sf::RenderTarget& target) const
{
    target.draw(rectangle);
}
