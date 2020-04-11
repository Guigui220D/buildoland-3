#include "GuiLabel.h"

#include "../Game.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "../../client-source/Settings/LanguageManager.h"
#include "../../client-source/Res/ResourceManager.h"

GuiLabel::~GuiLabel()
{

}

void GuiLabel::init()
{
    text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    text.setCharacterSize(200);
    text.scale(sf::Vector2f(.4f, .4f));

    sf::Vector2f text_size = sf::Vector2f(text.getLocalBounds().width, text.getLocalBounds().height);
    sf::Vector2f correction(0.f, 30.f);
    text.setOrigin(text_size / 2.f + correction);
}

void GuiLabel::draw(sf::RenderTarget& target) const
{
    target.draw(text);
}
