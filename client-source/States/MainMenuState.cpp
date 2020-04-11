#include "MainMenuState.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../../client-source/Settings/SettingsManager.h"

#include "../Game.h"
#include "LoadingScreenState.h"
#include "GameState.h"

MainMenuState::MainMenuState(Game& game, unsigned int id) :
    State(game, id),
    gui_zone(game, sf::Vector2f(.8f, .3f), GuiZone::Center, GuiZone::Middle),
    test_button_1(new GuiButton(game, sf::Vector2f(), 800.f, "SINGLEPLAYER_BUTTON")),
    test_button_2(new GuiButton(game, sf::Vector2f(0.f, 120.f), 800.f, "MULTIPLAYER_BUTTON"))
{
    update_transparent = false;
}

MainMenuState::~MainMenuState()
{
    //dtor
}

void MainMenuState::init()
{
    gui_zone.addElement(test_button_1);
    gui_zone.addElement(test_button_2);
    gui_zone.init();
}

bool MainMenuState::handleEvent(sf::Event& event)
{
    if (gui_zone.handleEvent(event))
        return true;

    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        return true;
    case sf::Event::Resized:
        gui_zone.calculateView(sf::Vector2u(event.size.width, event.size.height));
        return false;
    default:
        return false;
    }
}

void MainMenuState::update(float delta_time)
{
    gui_zone.update(delta_time);

    if (test_button_1->hasBeenClicked())
        getGame().addStateOnTop(new LoadingScreenState<GameState>(true, true, getGame(), 0));

    if (test_button_2->hasBeenClicked())
        getGame().addStateOnTop(new LoadingScreenState<GameState>(true, true, getGame(), 0, sf::IpAddress(getGame().getSettingsManager().getString("online_server_address")), 58888));
}

void MainMenuState::draw(sf::RenderTarget& target) const
{
    //gui_zone.debugDraw(target);
    gui_zone.draw(target);
}

void MainMenuState::updateView()
{
    gui_zone.calculateView(getGame().getWindow().getSize());
}
