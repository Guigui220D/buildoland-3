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

void State::init() {}

bool State::handleEvent(sf::Event& event) { return false; }

void State::update(float delta_time) {}

void State::draw(sf::RenderTarget& target) const {}

void State::updateView() {}
