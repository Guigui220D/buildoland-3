#include "GuiImage.h"

#include "../Game.h"

GuiImage::GuiImage(Game& game, sf::FloatRect zone, sf::Vector2f size, GuiAlign horizontal_align, GuiAlign vertical_align, const std::string texture_path, bool use_resource_manager) :
    GuiElement(game, zone, size, horizontal_align, vertical_align),
    texture_path(texture_path),
    use_manager(use_resource_manager)
{

}

GuiImage::~GuiImage()
{
    if (!use_manager)
        delete texture;
}

void GuiImage::init()
{
    if (use_manager)
    {
        texture = const_cast<sf::Texture*>(&getGame().getResourceManager().getTexture(texture_path));
    }
    else
    {
        texture = new sf::Texture();
        texture->loadFromFile(texture_path);
    }
    rectangle.setTexture(texture);
    rectangle.setSize(getSize());
}

void GuiImage::draw(sf::RenderTarget& target) const
{
    useView(target);
    target.draw(rectangle);
}
