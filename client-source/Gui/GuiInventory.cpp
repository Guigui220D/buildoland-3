#include "GuiInventory.h"

#include "../Game.h"

GuiInventory::GuiInventory(Game& game, PlayerInventory& inv) :
    GuiElement(game, sf::FloatRect(.2f, .2f, .6f, .6f), sf::Vector2f(110.f, 74.f), GuiAlign::Center, GuiAlign::Center),
    inventory(inv)
{
    //ctor
}

GuiInventory::~GuiInventory()
{
    //dtor
}

bool GuiInventory::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), getView());
        pos -= sf::Vector2f(1.f, 1.f);
        pos /= 18.f;

        sf::Vector2i i_pos(pos.x, pos.y);

        if (i_pos.x < 0 || i_pos.y < 0 || i_pos.x >= 6 || i_pos.y >= 4)
            return true;

        inventory.swapHands(i_pos.x + i_pos.y * 6 + 1);

        return true;
    }
    return false;
}

void GuiInventory::draw(sf::RenderTarget& target) const
{
    useView(target);
    target.draw(rectangle);

    for (int x = 0; x < 6; x++)
    for (int y = 0; y < 4; y++)
    {
        ItemStack& stack = inventory.contents.at(x + y * 6 + 1);

        if (!stack)
            continue;

        item_draw.setTextureRect(stack.getItem()->getTexture(stack));
        item_draw.setPosition(sf::Vector2f(18.f * x + 2.f, 18.f * y + 2.f));

        switch (stack.getItem()->getTexturesSet())
        {
        case Item::BlocksTextureSet:
            item_draw.setTexture(block_textures);
            break;
        case Item::GroundsTextureSet:
            item_draw.setTexture(ground_textures);
            break;
        case Item::ItemsTextureSet:
            item_draw.setTexture(item_textures);
            break;
        }

        target.draw(item_draw);

        if (stack.getAmount() > 1)
        {
            number_draw.setString(std::to_string(stack.getAmount()));
            number_draw.setPosition(sf::Vector2f(18.f * x + 3.f, 18.f * y + 2.f));
            target.draw(number_draw);
        }
    }

    ItemStack& hand = inventory.contents.at(0);
    if (hand)
    {
        item_hand.setTextureRect(hand.getItem()->getTexture(hand));
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow()));
        item_hand.setPosition(pos);
        number_draw.setPosition(pos + sf::Vector2f(-8.f, 0));

        switch (hand.getItem()->getTexturesSet())
        {
        case Item::BlocksTextureSet:
            item_hand.setTexture(block_textures);
            break;
        case Item::GroundsTextureSet:
            item_hand.setTexture(ground_textures);
            break;
        case Item::ItemsTextureSet:
            item_hand.setTexture(item_textures);
            break;
        }

        number_draw.setString(std::to_string(hand.getAmount()));

        target.draw(number_draw);
        target.draw(item_hand);
    }

    {
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow()));
        pos -= sf::Vector2f(1.f, 1.f);
        pos /= 18.f;

        sf::Vector2i i_pos(pos.x, pos.y);

        if (i_pos.x < 0 || i_pos.y < 0 || i_pos.x >= 6 || i_pos.y >= 4)
        {}
        else
        {
            ItemStack& stack = inventory.contents.at(i_pos.x + i_pos.y * 6 + 1);

            if (stack)
            {
                hover_text.setString(stack.getItem()->getDisplayName());
                hover_text.setPosition(getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow())) - sf::Vector2f(0, 4.f));
                target.draw(hover_text);
            }
        }
    }


}

void GuiInventory::init()
{
    texture = &getGame().getResourceManager().getTexture("INVENTORY");
    rectangle.setTexture(texture);
    rectangle.setSize(getSize());
    rectangle.setFillColor(sf::Color(255, 255, 255, 200));

    item_draw.setSize(sf::Vector2f(16.f, 16.f));
    item_hand.setSize(sf::Vector2f(8.f, 8.f));
    item_hand.setOrigin(sf::Vector2f(8.f, 8.f));

    block_textures = &getGame().getResourceManager().getTexture("BLOCK_TEXTURES");
    ground_textures = &getGame().getResourceManager().getTexture("GROUND_TEXTURES");
    item_textures = &getGame().getResourceManager().getTexture("ITEM_TEXTURES");

    number_draw.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    number_draw.setString("foo");
    number_draw.setCharacterSize(100);
    number_draw.scale(sf::Vector2f(.04f, .04f));

    hover_text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    hover_text.setString("foo");
    hover_text.setCharacterSize(100);
    hover_text.scale(sf::Vector2f(.04f, .04f));
    hover_text.setOutlineColor(sf::Color::Black);
    hover_text.setOutlineThickness(4.f);
}

void GuiInventory::update(float delta_time)
{

}
