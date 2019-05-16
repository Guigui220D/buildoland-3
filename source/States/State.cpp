#include "State.h"

State::State(Game* game, unsigned int id) :
    game(game),
    id(id)
{
    update_transparent  = true;
    draw_transparent    = true;
}

State::~State()
{
    //dtor
}

bool State::handleEvent(sf::Event& event)
{
    return false;
}

void State::update(float delta_time) {}

void State::draw(sf::RenderTarget& target) const
{
    //Test
        sf::RectangleShape rs;
        rs.setSize(sf::Vector2f(100.f, 50.f));
        rs.setFillColor(sf::Color::Red);
        target.draw(rs);
}
