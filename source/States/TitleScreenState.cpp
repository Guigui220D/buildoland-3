#include "TitleScreenState.h"

TitleScreenState::TitleScreenState(Game* game, unsigned int id) :
    LoadingScreenState<MainMenuState>(false, true, game, id),
    title(game)
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
}

bool TitleScreenState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::Resized)
        title.calculateView(getGame()->getWindow().getSize());
    return true;
}

void TitleScreenState::beforeInitTask()
{
    //We can access that because Game has TitleScreenState as a friend
    this->getGame()->loadResources();
    this->getGame()->gameBlocksManager.initBlocks();
    this->getGame()->gameGroundsManager.initGrounds();
}

void TitleScreenState::afterInitTask()
{
    while (clk.getElapsedTime().asSeconds() < 3.f); //Because we want to see the title
}

void TitleScreenState::drawMore(sf::RenderTarget& target) const
{
    title.draw(target);
}

void TitleScreenState::updateMore(float delta_time)
{
    //title.update(delta_time);
    title.setFade(getFade());
}

