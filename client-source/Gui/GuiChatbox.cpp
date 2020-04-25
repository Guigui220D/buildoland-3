#include "GuiChatbox.h"

#include "../Game.h"
#include "../../client-source/Res/ResourceManager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

    // palette : https://coolors.co/805d93-f49fbc-ffd3ba-9ebd6e-169873
    static const sf::Color cool_colors[] =
        {
            sf::Color(0xFFD3BAFF),
            sf::Color(0x9EBD6EFF),
            sf::Color(0x169873FF),
            sf::Color(0x805D93FF),
            sf::Color(0xF49FBCFF),
            };

GuiChatbox::GuiChatbox(Game &game, sf::Vector2f pos, float width)
    : GuiElement(game), position(pos)
{
    //BACKGROUND
    background = sf::RectangleShape();
    background.setFillColor(sf::Color(100, 100, 100, 150));
    background.setOutlineColor(sf::Color(200, 200, 200, 100));
    background.setOutlineThickness(1.f);
    background.setPosition(pos.x, pos.y);

    setSize(sf::Vector2f(width, 100.f));
}

void GuiChatbox::addMessage(const std::string &nickname, const sf::String &message)
{
    // clear outdated messages
    while (messages.size() >= max_messages)
    {
        messages.pop_front();
    }

    if (nickname_colors.find(nickname) == nickname_colors.end())
        nickname_colors[nickname] = getNewColor();

    sf::Color color = nickname_colors[nickname];

    sf::Text text;
    text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    text.setFillColor(color);
    text.setCharacterSize(18);
    text.setString(sf::String("<" + nickname + ">: ") + message);

    //text.setOutlineThickness(0.5f);
    text.setOutlineColor(sf::Color::Black);
    text.setPosition(0, 0);
    fitText(text);
    text.setPosition(position.x + 5.f, position.y + background.getSize().y - text.getLocalBounds().height - 15.f);

    // move previous messages up
    for (auto& prev_msg : messages)
        prev_msg.move(0, -text.getLocalBounds().height - 4.f);

    messages.push_back(text);
}

sf::Color GuiChatbox::getNewColor()
{
    sf::Color color = cool_colors[next_color_idx];
    ++next_color_idx;
    return color;
}

void GuiChatbox::fitText(sf::Text &text)
{
    for (size_t i = 1; i < text.getString().getSize(); ++i)
    {
        size_t word_start = i;
        // scan the word
        while (i < text.getString().getSize() && iswalpha(text.getString()[i]))
            ++i;
        // end of word pos
        sf::Vector2f pos = text.findCharacterPos(i-1);
        sf::Glyph glyph = text.getFont()->getGlyph(text.getString()[i-1], text.getCharacterSize(), false, text.getOutlineThickness());
        // add a line break
        if (pos.x + glyph.bounds.width >= background.getSize().x)
        {
            sf::String str = text.getString();
            str.insert(word_start, '\n');
            text.setString(str);
        }
    }
}

void GuiChatbox::update(float dt)
{
    (void)dt;
}

void GuiChatbox::draw(sf::RenderTarget &target) const
{
    target.draw(background);

    sf::View old_view = target.getView();
    sf::View clip_view = sf::View(sf::FloatRect(position.x, position.y - 4.f*scroll_val, background.getSize().x, background.getSize().y));
    clip_view.setViewport(sf::FloatRect(position.x / target.getSize().x, position.y / target.getSize().y, background.getSize().x / target.getSize().x, background.getSize().y / target.getSize().y));

    target.setView(clip_view);
    for (const auto& text : messages)
        target.draw(text);
    target.setView(old_view);
}

bool GuiChatbox::handleEvent(sf::Event &e)
{
    switch (e.type)
    {
        case sf::Event::MouseWheelScrolled:
            if (background.getGlobalBounds().contains(e.mouseWheelScroll.x, e.mouseWheelScroll.y))
            {
                scroll_val += e.mouseWheelScroll.delta;
                if (scroll_val < 0)
                    scroll_val = 0;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}
