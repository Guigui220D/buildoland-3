#include "GuiInventory.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "../../client-source/Settings/LanguageManager.h"
#include "../../client-source/Settings/BindingsManager.h"
#include "../../client-source/Res/ResourceManager.h"

#include "../../common-source/Items/PlayerInventory.h"

#include "../Game.h"

float constexpr operator""_p(long double arg)
    { return arg * (75.f / 11.f); } //Magic number. Because the image is 110 pixels wide but is fit on a 750 units area

GuiInventory::GuiInventory(Game& game, PlayerInventory& inv) :
    GuiElement(game),
    inventory(inv)
{
    for (int set = 0; set < Item::TextureSetEnd; ++set)
        vertex_arrays[set].setPrimitiveType(sf::PrimitiveType::Quads);
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
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)/*, getView()*/);
        pos -= sf::Vector2f(1.0_p, 7.0_p);
        pos /= 18.0_p;

        sf::Vector2i i_pos(pos.x, pos.y);

        if (i_pos.x < 0 || i_pos.y < 0 || i_pos.x >= 6 || i_pos.y >= 4)
            return true;

        inventory.swapHands(i_pos.x + i_pos.y * 6 + 1);

        return true;
    }
    else if (getGame().getBindingsManager().pressed("throw_item"))
    {
        inventory.dropHand();

        return true;
    }

    return false;
}

void GuiInventory::draw(sf::RenderTarget& target) const
{
    target.draw(rectangle);
    target.draw(title);

    // render the vertex arrays once
    sf::RenderStates states = sf::RenderStates::Default;
    states.texture = block_textures;
    target.draw(vertex_arrays[Item::BlocksTextureSet], states);
    states.texture = ground_textures;
    target.draw(vertex_arrays[Item::GroundsTextureSet], states);
    states.texture = item_textures;
    target.draw(vertex_arrays[Item::ItemsTextureSet], states);

    vertex_arrays[Item::BlocksTextureSet].clear();
    vertex_arrays[Item::GroundsTextureSet].clear();
    vertex_arrays[Item::ItemsTextureSet].clear();

    // draw the amount numbers
    for (int x = 0; x < 6; x++)
        for (int y = 0; y < 4; y++)
        {
            ItemStack& stack = inventory.getStack(x + y * 6 + 1);
            if (!stack)
                continue;

            if (stack.getAmount() > 1)
            {
                number_draw.setString(std::to_string(stack.getAmount()));
                number_draw.setPosition(sf::Vector2f(18.0_p * x + 3.0_p, 18.0_p * y + 8.0_p + 9.0_p));
                target.draw(number_draw);
            }
        }

    {
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow()));
        pos -= sf::Vector2f(1.0_p, 7.0_p);
        pos /= 18.0_p;

        sf::Vector2i i_pos(pos.x, pos.y);

        if (pos.x < 0 || pos.y < 0 || i_pos.x < 0 || i_pos.y < 0 || i_pos.x >= 6 || i_pos.y >= 4)
        {}
        else
        {
            ItemStack& stack = inventory.getStack(i_pos.x + i_pos.y * 6 + 1);

            if (stack)
            {
                hover_text.setString(stack.getItem()->getDisplayName());
                hover_text.setPosition(18.0_p * i_pos.x + 1.0_p, 18.0_p * i_pos.y + 7.0_p);
                target.draw(hover_text);
            }
        }
    }

    // draw the hand item on top of everything
    ItemStack& hand = inventory.getHand();
    if (hand)
    {
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow()));
        number_draw.setPosition(pos + sf::Vector2f(-8.0_p, 8.0_p) / 2.f);

        sf::Vertex vertices[4];
        vertices[0].position = pos - sf::Vector2f(8.0_p, 8.0_p) / 2.f;
        vertices[1].position = vertices[0].position + sf::Vector2f(8.0_p, 0);
        vertices[2].position = vertices[0].position + sf::Vector2f(8.0_p, 8.0_p);
        vertices[3].position = vertices[0].position + sf::Vector2f(0, 8.0_p);
        hand.getItem()->addTexturedVertices(hand, vertices);

        for (int i = 0; i < 4; ++i)
            vertex_arrays[hand.getItem()->getTexturesSet()].append(vertices[i]);

        number_draw.setString(std::to_string(hand.getAmount()));
    }
    states = sf::RenderStates::Default;
    states.texture = block_textures;
    target.draw(vertex_arrays[Item::BlocksTextureSet], states);
    states.texture = ground_textures;
    target.draw(vertex_arrays[Item::GroundsTextureSet], states);
    states.texture = item_textures;
    target.draw(vertex_arrays[Item::ItemsTextureSet], states);

    if (hand)
        target.draw(number_draw);
}

void GuiInventory::init()
{
    title.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    title.setString(getGame().getLanguageManager().getString("INVENTORY_TITLE"));
    title.setCharacterSize(100);
    title.scale(sf::Vector2f(.4f, .4f));
    title.setPosition(sf::Vector2f(10.f, -5.f));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(4.f);

    texture = &getGame().getResourceManager().getTexture("INVENTORY");
    rectangle.setTexture(texture);
    rectangle.setSize(sf::Vector2f(110.0_p, 80.0_p));
    rectangle.setFillColor(sf::Color(255, 255, 255, 200));

    block_textures = &getGame().getResourceManager().getTexture("BLOCK_TEXTURES");
    ground_textures = &getGame().getResourceManager().getTexture("GROUND_TEXTURES");
    item_textures = &getGame().getResourceManager().getTexture("ITEM_TEXTURES");

    number_draw.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    number_draw.setString("foo");
    number_draw.setCharacterSize(100);
    number_draw.scale(sf::Vector2f(.4f, .4f));
    number_draw.setOutlineColor(sf::Color::Black);
    number_draw.setOutlineThickness(4.f);

    hover_text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    hover_text.setString("bar");
    hover_text.setCharacterSize(100);
    hover_text.scale(sf::Vector2f(.4f, .4f));
    hover_text.setOutlineColor(sf::Color::Black);
    hover_text.setOutlineThickness(4.f);
}

void GuiInventory::update(float delta_time)
{
    for (int set = 0; set < Item::TextureSetEnd; ++set)
        vertex_arrays[set].clear();

    for (int x = 0; x < 6; x++)
        for (int y = 0; y < 4; y++)
        {
            ItemStack& stack = inventory.getStack(x + y * 6 + 1);

            if (!stack)
                continue;

            sf::Vertex vertices[4];
            vertices[0].position = sf::Vector2f(18.0_p * x + 2.0_p, 18.0_p * y + 8.0_p);
            vertices[1].position = vertices[0].position + sf::Vector2f(16.0_p, 0);
            vertices[2].position = vertices[0].position + sf::Vector2f(16.0_p, 16.0_p);
            vertices[3].position = vertices[0].position + sf::Vector2f(0, 16.0_p);
            stack.getItem()->addTexturedVertices(stack, vertices);

            for (int i = 0; i < 4; ++i)
                vertex_arrays[stack.getItem()->getTexturesSet()].append(vertices[i]);
        }
}
