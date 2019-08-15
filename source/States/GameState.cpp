#include "GameState.h"

#include "../Game.h"

GameState::GameState(Game* game, unsigned int id) :
    State(game, id),
    test_world(game),
    my_view(sf::Vector2f(4.f, 4.f), sf::Vector2f(20.f, 20.f))
{
    update_transparent = false;
    draw_transparent = false;
    updateView();

    test_world.getChunk(sf::Vector2i(0, 0));
}

GameState::~GameState()
{
    //dtor
}

bool GameState::handleEvent(sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::Resized:
        updateView();
        return true;

    case sf::Event::KeyPressed:
        test_world.getChunk(sf::Vector2i(0, 0)).regenerate();
        return true;

    default:
        return true;
    }
}

void GameState::update(float delta_time)
{

}

void GameState::draw(sf::RenderTarget& target) const
{
    target.setView(my_view);

    const Chunk& chunk = test_world.getChunkConst(sf::Vector2i(0, 0));

    target.draw(chunk.getGroundVertexArray(), &getGame()->getResourceManager().getTexture("GROUND_TEXTURES"));
    target.draw(chunk.getGroundDetailsVertexArray(), &getGame()->getResourceManager().getTexture("GROUND_DETAILS"));
}

void GameState::updateView()
{
    sf::RenderWindow& window = getGame()->getWindow();
    //Resize without deformation
	if (window.getSize().x < 200)
		window.setSize(sf::Vector2u(200, window.getSize().y));
	if (window.getSize().y < 200)
		window.setSize(sf::Vector2u(window.getSize().x, 200));
	if (window.getSize().y > window.getSize().x)
	{
		float ratio = (float)window.getSize().y / window.getSize().x;
		float y_size = ratio * 10;
		my_view.setSize(sf::Vector2f(10, y_size));
	}
	else
	{
		float ratio = (float)window.getSize().x / window.getSize().y;
		float x_size = ratio * 10;
		my_view.setSize(sf::Vector2f(x_size, 10));
	}
}
