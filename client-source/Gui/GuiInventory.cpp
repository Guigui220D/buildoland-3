#include "GuiInventory.h"

#include "../Game.h"

GuiInventory::GuiInventory(Game& game, PlayerInventory& inv) :
    GuiElement(game, sf::FloatRect(.2f, .2f, .6f, .6f), sf::Vector2f(110.f, 80.f), GuiAlign::Center, GuiAlign::Center),
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
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), getView());
        pos -= sf::Vector2f(1.f, 7.f);
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
    target.draw(title);

    for (int x = 0; x < 6; x++)
    for (int y = 0; y < 4; y++)
    {
        ItemStack& stack = inventory.contents.at(x + y * 6 + 1);
        if (!stack)
            continue;

        if (stack.getAmount() > 1)
        {
            number_draw.setString(std::to_string(stack.getAmount()));
            number_draw.setPosition(sf::Vector2f(18.f * x + 3.f, 18.f * y + 8.f));
            target.draw(number_draw);
        }
    }

    ItemStack& hand = inventory.contents.at(0);
    if (hand)
    {
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow()));
        number_draw.setPosition(pos + sf::Vector2f(-8.f, 8.f)/2.f);

        sf::Vertex vertices[4];
        vertices[0].position = pos - sf::Vector2f(8.f, 8.f)/2.f;
        vertices[1].position = vertices[0].position + sf::Vector2f(8.f, 0);
        vertices[2].position = vertices[0].position + sf::Vector2f(8.f, 8.f);
        vertices[3].position = vertices[0].position + sf::Vector2f(0, 8.f);
        hand.getItem()->addTexturedVertices(hand, vertices);

        for (int i = 0; i < 4; ++i)
            vertex_arrays[hand.getItem()->getTexturesSet()].append(vertices[i]);

        number_draw.setString(std::to_string(hand.getAmount()));
    }

    // render the vertex arrays once
    sf::RenderStates states = sf::RenderStates::Default;
    states.texture = block_textures;
    target.draw(vertex_arrays[Item::BlocksTextureSet], states);
    states.texture = ground_textures;
    target.draw(vertex_arrays[Item::GroundsTextureSet], states);
    states.texture = item_textures;
    target.draw(vertex_arrays[Item::ItemsTextureSet], states);

    target.draw(number_draw);

    // draw the amount numbers
    for (int x = 0; x < 6; x++)
        for (int y = 0; y < 4; y++)
        {
            ItemStack& stack = inventory.contents.at(x + y * 6 + 1);
            if (!stack)
                continue;

            if (stack.getAmount() > 1)
            {
                number_draw.setString(std::to_string(stack.getAmount()));
                number_draw.setPosition(sf::Vector2f(18.f * x + 3.f, 18.f * y + 8.f));
                target.draw(number_draw);
            }
        }

    {
        sf::Vector2f pos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow()));
        pos -= sf::Vector2f(1.f, 7.f);
        pos /= 18.f;

        sf::Vector2i i_pos(pos.x, pos.y);

        if (pos.x < 0 || pos.y < 0 || i_pos.x < 0 || i_pos.y < 0 || i_pos.x >= 6 || i_pos.y >= 4)
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
    title.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    title.setString(getGame().getLanguageManager().getString("INVENTORY_TITLE"));
    title.setCharacterSize(100);
    title.scale(sf::Vector2f(.04f, .04f));
    title.setPosition(sf::Vector2f(2.f, 1.f));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(4.f);

    texture = &getGame().getResourceManager().getTexture("INVENTORY");
    rectangle.setTexture(texture);
    rectangle.setSize(getSize());
    rectangle.setFillColor(sf::Color(255, 255, 255, 200));

    block_textures = &getGame().getResourceManager().getTexture("BLOCK_TEXTURES");
    ground_textures = &getGame().getResourceManager().getTexture("GROUND_TEXTURES");
    item_textures = &getGame().getResourceManager().getTexture("ITEM_TEXTURES");

    number_draw.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    number_draw.setString("foo");
    number_draw.setCharacterSize(100);
    number_draw.scale(sf::Vector2f(.04f, .04f));
    number_draw.setOutlineColor(sf::Color::Black);
    number_draw.setOutlineThickness(4.f);

    hover_text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    hover_text.setString("foo");
    hover_text.setCharacterSize(100);
    hover_text.scale(sf::Vector2f(.04f, .04f));
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
            ItemStack& stack = inventory.contents.at(x + y * 6 + 1);

            if (!stack)
                continue;

            sf::Vertex vertices[4];
            vertices[0].position = sf::Vector2f(18.f * x + 2.f, 18.f * y + 8.f);
            vertices[1].position = vertices[0].position + sf::Vector2f(16.f, 0);
            vertices[2].position = vertices[0].position + sf::Vector2f(16.f, 16.f);
            vertices[3].position = vertices[0].position + sf::Vector2f(0, 16.f);
            stack.getItem()->addTexturedVertices(stack, vertices);

            for (int i = 0; i < 4; ++i)
                vertex_arrays[stack.getItem()->getTexturesSet()].append(vertices[i]);
        }
}
