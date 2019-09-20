#include "TitleScreenState.h"

TitleScreenState::TitleScreenState(Game* game, unsigned int id) :
    LoadingScreenState<MainMenuState>(false, true, game, id),
    title(game),
    sfml(game, sf::FloatRect(.05f, .8f, .15f, .15f), (1001.f / 304.f), GuiAlign::BottomOrRight, GuiAlign::BottomOrRight, "Res/Logos/sfml_logo_big.png", false)
{
    //ctor
}

TitleScreenState::~TitleScreenState()
{
    //dtor
}

void TitleScreenState::init()
{
    title.init();
    sfml.init();
}

bool TitleScreenState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::Resized)
    {
        title.calculateView(getGame()->getWindow().getSize());
        sfml.calculateView(getGame()->getWindow().getSize());
        loading_icon.calculateView(getGame()->getWindow().getSize());
    }
    return true;
}

void TitleScreenState::beforeInitTask()
{
    //We can access that because Game has TitleScreenState as a friend
    this->getGame()->loadResources();
    this->getGame()->game_blocks_manager.initBlocks();
    this->getGame()->game_grounds_manager.initGrounds();
}

void TitleScreenState::afterInitTask()
{
    while (clk.getElapsedTime().asSeconds() < 2.f); //Because we want to see the title
}

void TitleScreenState::drawMore(sf::RenderTarget& target) const
{
    title.draw(target);
    sfml.draw(target);
}

void TitleScreenState::updateMore(float delta_time)
{
    //title.update(delta_time);
    title.setFade(getFade());
    sfml.setTransparency(getFade());
}

