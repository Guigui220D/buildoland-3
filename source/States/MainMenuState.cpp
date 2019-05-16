#include "MainMenuState.h"

MainMenuState::MainMenuState(Game* game, unsigned int id) :
    State(game, id),
    test_ge_1(sf::Vector2u(800, 600), sf::FloatRect(0.1, 0.5, 0.8, 0.1), 8, GuiAlign::Center, GuiAlign::Center)
{
    update_transparent = false;
}

MainMenuState::~MainMenuState()
{
    //dtor
}

bool MainMenuState::handleEvent(sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
        //mustBeDestroyed = true;
        return true;
    case sf::Event::Resized:
        test_ge_1.calculateView(sf::Vector2u(event.size.width, event.size.height));
        return false;
    default:
        return false;
    }
}

void MainMenuState::draw(sf::RenderTarget& target) const
{
    test_ge_1.debugDraw(target);
}
