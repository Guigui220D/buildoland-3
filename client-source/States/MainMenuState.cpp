#include "MainMenuState.h"

#include "../Game.h"
#include "LoadingScreenState.h"
#include "GameState.h"

MainMenuState::MainMenuState(Game* game, unsigned int id) :
    State(game, id),
    test_button_1(new GuiButton(game, sf::FloatRect(.1f, .5f, .8f, .1f), 8.f, GuiAlign::Center, GuiAlign::BottomOrRight, "Singleplayer")),
    test_button_2(new GuiButton(game, sf::FloatRect(.1f, .7f, .8f, .1f), 8.f, GuiAlign::Center, GuiAlign::TopOrLeft, "Multiplayer")),
    serv_debug_checkbox(new GuiCheckBox(game, sf::FloatRect(.85f, .5f, .1f, .1f), GuiAlign::Center, GuiAlign::BottomOrRight))
{
    update_transparent = false;

    gui_manager.push_back(test_button_1);
    gui_manager.push_back(test_button_2);
    gui_manager.push_back(serv_debug_checkbox);
}

MainMenuState::~MainMenuState()
{
    //dtor
}

void MainMenuState::init()
{
    gui_manager.initEverything();
}

bool MainMenuState::handleEvent(sf::Event& event)
{
    if (gui_manager.handleEvent(event))
        return true;

    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        return true;
    case sf::Event::Resized:
        gui_manager.updateWindowSize(sf::Vector2u(event.size.width, event.size.height));
        return false;
    default:
        return false;
    }
}

void MainMenuState::update(float delta_time)
{
    gui_manager.updateEverything(delta_time);

    if (test_button_1->hasBeenClicked())
        getGame()->addStateOnTop(new LoadingScreenState<GameState>(true, true, getGame(), 0));

    if (test_button_2->hasBeenClicked())
        getGame()->addStateOnTop(new LoadingScreenState<GameState>(true, true, getGame(), 0, sf::IpAddress::LocalHost, 58888));
}

void MainMenuState::draw(sf::RenderTarget& target) const
{
    gui_manager.drawEverything(target);
}

void MainMenuState::updateView()
{
    gui_manager.updateWindowSize(getGame()->getWindow().getSize());
}
