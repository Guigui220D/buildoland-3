#pragma once

#include <list>
#include <unordered_map>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../../client-source/Settings/LanguageManager.h"
#include "../../client-source/Game.h"

#include "GuiElement.h"

class GuiChatbox : public GuiElement
{
public:
    GuiChatbox(Game& game, sf::Vector2f pos, float width);

    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override ;
    bool handleEvent(sf::Event& e) override;

    void setSize(const sf::Vector2f& vec)
    {
        background.setSize(vec);
    }
    void setPosition(float x, float y)
    {
        position = {x, y};
        background.setPosition(x, y);
    }
    sf::Vector2f getPosition() const
    { return position; }

    template <typename... FmtArgs>
    void addInfoMessage(const std::string& str, FmtArgs&&... args)
    {
        addMessage("", game.getLanguageManager().getString(str, std::forward<FmtArgs>(args)...));
    }
    void addMessage(const std::string& nickname, const sf::String& message);
private:
    void cleanOlderMessages();
    sf::Color getNewColor();
    void fitText(sf::Text& text);

private:
    static constexpr int max_messages = 100;

    std::list<sf::Text> messages;
    sf::Vector2f position;

    int next_color_idx = 0;
    float scroll_val = 0;
    std::unordered_map<std::string, sf::Color> nickname_colors;

    sf::RectangleShape background;
};
