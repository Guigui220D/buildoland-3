#pragma once

#include <vector>
#include <cstdint>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/View.hpp>

namespace sf
{
    class Event;
}

class Game;
class GuiElement;

class GuiZone
{
    public:
        enum HAlign
        {
            Left,
            Center,
            Right
        };

        enum VAlign
        {
            Top,
            Middle,
            Bottom
        };

        GuiZone(Game& game, sf::Vector2f size_percent, HAlign horizontal_align, VAlign vertical_align);
        ~GuiZone();

        void calculateView(sf::Vector2u window_size);

        void debugDraw(sf::RenderTarget& target) const;

        void init();
        bool handleEvent(sf::Event& event);
        void update(float delta_time);
        void draw(sf::RenderTarget& target) const;
        void setTransparency(uint8_t fade);

        inline void addElement(GuiElement* element) { elements.push_back(element); }

    private:
        Game& game;

        sf::View my_view;

        HAlign h_align;
        VAlign v_align;

        sf::Vector2f size_portion;
        sf::Vector2f usable_size;

        sf::RectangleShape test_rect;
        sf::CircleShape test_2;

        std::vector<GuiElement*> elements;
};
