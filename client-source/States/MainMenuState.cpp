#include "MainMenuState.h"

#include "../Game.h"
#include "LoadingScreenState.h"
#include "GameState.h"

MainMenuState::MainMenuState(Game* game, unsigned int id) :
    State(game, id),
    test_button_1(game, sf::FloatRect(0.1, 0.5, 0.8, 0.1), 8, GuiAlign::Center, GuiAlign::BottomOrRight, "Singleplayer"),
    test_button_2(game, sf::FloatRect(0.1, 0.7, 0.8, 0.1), 8, GuiAlign::Center, GuiAlign::TopOrLeft, "Multiplayer")
{
    update_transparent = false;
}

MainMenuState::~MainMenuState()
{
    //dtor
}

void MainMenuState::init()
{
    test_button_1.init();
    test_button_2.init();
}

bool MainMenuState::handleEvent(sf::Event& event)
{
    if (test_button_1.handleEvent(event))
        return true;
    if (test_button_2.handleEvent(event))
        return true;

    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        return true;
    case sf::Event::Resized:
        test_button_1.calculateView(sf::Vector2u(event.size.width, event.size.height));
        test_button_2.calculateView(sf::Vector2u(event.size.width, event.size.height));
        return false;
    default:
        return false;
    }
}

void MainMenuState::update(float delta_time)
{
    test_button_1.update(delta_time);
    test_button_2.update(delta_time);

    if (test_button_1.hasBeenClicked())
        getGame()->addStateOnTop(new LoadingScreenState<GameState>(true, true, getGame(), 0));

    if (test_button_2.hasBeenClicked())
        getGame()->addStateOnTop(new LoadingScreenState<GameState>(true, true, getGame(), 0, sf::IpAddress::LocalHost, 58888));
}

void MainMenuState::draw(sf::RenderTarget& target) const
{
    test_button_1.draw(target);
    test_button_2.draw(target);
}

void MainMenuState::updateView()
{
    test_button_1.calculateView(getGame()->getWindow().getSize());
    test_button_2.calculateView(getGame()->getWindow().getSize());
}
