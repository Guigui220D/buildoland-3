#include "TitleScreenState.h"

#include "../../common-source/Utils/Log.h"

TitleScreenState::TitleScreenState(Game& game, unsigned int id) :
    LoadingScreenState<MainMenuState>(false, true, game, id),
    middle_gui_zone(game, sf::Vector2f(.75f, .2f), GuiZone::Center, GuiZone::Middle),
    bottom_left_gui_zone(game, sf::Vector2f(.25f, .1f), GuiZone::Left, GuiZone::Bottom),
    title(new GuiImage(game, sf::Vector2f(), sf::Vector2f(750.f, 102.75f), "Resources/Logos/text_version_2.png", false, true)),
    sfml(new GuiImage(game, sf::Vector2f(50.f, 0.f), sf::Vector2f(200.f, 66.66f), "Resources/Logos/sfml_logo_big.png", false, true))
{
    //ctor
}

TitleScreenState::~TitleScreenState()
{
    //dtor
}

void TitleScreenState::init()
{
    middle_gui_zone.addElement(title);
    bottom_left_gui_zone.addElement(sfml);

    middle_gui_zone.init();
    bottom_left_gui_zone.init();
}

bool TitleScreenState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::Resized)
    {
        middle_gui_zone.calculateView(sf::Vector2u(event.size.width, event.size.height));
        bottom_left_gui_zone.calculateView(sf::Vector2u(event.size.width, event.size.height));
        return false;
    }
    return true;
}

void TitleScreenState::beforeInitTask()
{
    //We can access that because Game has TitleScreenState as a friend
    this->getGame().language_manager.load(this->getGame().settings_manager.getString("game_language"));
    this->getGame().loadResources();
    this->getGame().game_blocks_manager.initBlocks();
    this->getGame().game_grounds_manager.initGrounds();
    this->getGame().game_items_register.initItems(this->getGame().game_blocks_manager, this->getGame().game_grounds_manager);
}

void TitleScreenState::afterInitTask()
{
    log(INFO, "Title screen state finished in {}s.\n", clk.getElapsedTime().asSeconds());
    while (clk.getElapsedTime().asSeconds() < 2.f); //Because we want to see the title
}

void TitleScreenState::drawMore(sf::RenderTarget& target) const
{
    /*
    middle_gui_zone.debugDraw(target);
    bottom_left_gui_zone.debugDraw(target);
    */
    middle_gui_zone.draw(target);
    bottom_left_gui_zone.draw(target);
}

void TitleScreenState::updateMore(float delta_time)
{
    middle_gui_zone.update(delta_time);
    bottom_left_gui_zone.update(delta_time);
}

