#include "NicknameChoiceState.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Game.h"

#include "LoadingScreenState.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "../Settings/SettingsManager.h"
#include "../../common-source/Utils/UsernameCheck.h"

#include "../../common-source/Utils/Log.h"

NicknameChoiceState::NicknameChoiceState(Game &game, unsigned int id) :
      State(game, id),
      gui_zone(game, sf::Vector2f(.8f, .3f), GuiZone::Center, GuiZone::Middle),
      start_button(game, sf::Vector2f(0, 360.f), 800.f, "START_GAME"),
      main_menu_button(game, sf::Vector2f(0, 480.f), 800.f, "BACK_TO_MAIN_MENU_BUTTON"),
      error_label(game, sf::Vector2f(400.f, -260.f), "SERVER_ADDRESS"),
      ip_label(game, sf::Vector2f(400.f, -240.f), ""),
      ip_input(game, sf::Vector2f(0, -120.f), 800.f, "localhost"),
      nickname_label(game, sf::Vector2f(400.f, 40.f), "NICKNAME"),
      nickname_input(game, sf::Vector2f(0, 120.f), 800.f, "nickname")
{
    update_transparent = draw_transparent = false;

    error_label.setColor(sf::Color::Transparent);

    nickname_input.setValidator([](const sf::String& str)
                                {
                                    for (auto c : str)
                                        if (c > 127) // not an ascii char
                                            return false;
                                    return UsernameCheck::checkUsername(str.toAnsiString());
                                });

    // set to default address name
    ip_input.setInputText(getGame().getSettingsManager().getString("online_server_address"));
    nickname_input.setInputText(getGame().getSettingsManager().getString("online_player_name"));
}

NicknameChoiceState::~NicknameChoiceState()
{

}

void NicknameChoiceState::init()
{
    gui_zone.addElement(&error_label);
    gui_zone.addElement(&start_button);
    gui_zone.addElement(&nickname_label);
    gui_zone.addElement(&nickname_input);
    gui_zone.addElement(&ip_label);
    gui_zone.addElement(&ip_input);
    gui_zone.addElement(&main_menu_button);
    gui_zone.init();
}

bool NicknameChoiceState::handleEvent(sf::Event &event)
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

void NicknameChoiceState::update(float delta_time)
{
    gui_zone.update(delta_time);

    if (start_button.hasBeenClicked())
    {
        if (!nickname_input.valid())
        {
            error_label.setColor(sf::Color::Red);
            error_label.resetMessage("INVALID_NICKNAME");
        }
        else if (sf::IpAddress(ip_input.getInputText().toAnsiString()) == sf::IpAddress::None)
        {
            error_label.setColor(sf::Color::Red);
            error_label.resetMessage("INVALID_ADDRESS");
        }
        else
        {
            getGame().addStateOnTop(new LoadingScreenState<GameState>(true, true, getGame(), 0, nickname_input.getInputText().toAnsiString(), sf::IpAddress(ip_input.getInputText().toAnsiString()), 58888));
        }
    }

    if (main_menu_button.hasBeenClicked())
    {
        getGame().addStateOnTop(new LoadingScreenState<MainMenuState>(true, true, getGame(), 0));
    }
}

void NicknameChoiceState::draw(sf::RenderTarget &target) const
{
    gui_zone.draw(target);
}

void NicknameChoiceState::updateView()
{
    gui_zone.calculateView(getGame().getWindow().getSize());
}
