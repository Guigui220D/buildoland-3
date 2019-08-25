#include "GameState.h"

#include "../Game.h"

GameState::GameState(Game* game, unsigned int id) :
    State(game, id),
    test_world(game),
    my_view(sf::Vector2f(4.f, 4.f), sf::Vector2f(20.f, 20.f)),
    block_textures(&getGame()->getResourceManager().getTexture("BLOCK_TEXTURES")),
    ground_textures(&getGame()->getResourceManager().getTexture("GROUND_TEXTURES")),
    ground_details_textures(&getGame()->getResourceManager().getTexture("GROUND_DETAILS"))
{
    update_transparent = false;
    draw_transparent = false;
    updateView();


    test_world.getChunk(sf::Vector2i(0, 0));
    test_world.getChunk(sf::Vector2i(1, 0));
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
        {
            sf::RenderWindow& window = getGame()->getWindow();
            if (window.getSize().x < 200)
                window.setSize(sf::Vector2u(200, window.getSize().y));
            if (window.getSize().y < 200)
                window.setSize(sf::Vector2u(window.getSize().x, 200));
        }
        updateView();
        break;

    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::A)
            test_world.getChunk(sf::Vector2i(0, 0)).regenerate();
        if (event.key.code == sf::Keyboard::B)
            test_world.getChunk(sf::Vector2i(0, 0)).setGround(15, 7, GameGrounds::GRASS);
        if (event.key.code == sf::Keyboard::C)
            test_world.getChunk(sf::Vector2i(0, 0)).to_be_removed = true;
        break;

    case sf::Event::MouseWheelScrolled:
        zoom += event.mouseWheelScroll.delta;
        updateView();
        break;

    default:
        break;
    }
    return true;
}

void GameState::update(float delta_time)
{
    //TEMPORARY
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        my_view.setCenter(my_view.getCenter() + sf::Vector2f(-5.f * delta_time, 0));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        my_view.setCenter(my_view.getCenter() + sf::Vector2f(5.f * delta_time, 0));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        my_view.setCenter(my_view.getCenter() + sf::Vector2f(0, 5.f * delta_time));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        my_view.setCenter(my_view.getCenter() + sf::Vector2f(0, -5.f * delta_time));

    if (anim_clock.getElapsedTime().asSeconds() >= .5f)
    {
        anim_clock.restart();
        anim_frame = (anim_frame + 1) % 4;
    }

    test_world.updateLoadedChunk(my_view.getCenter());
}

void GameState::draw(sf::RenderTarget& target) const
{
    target.setView(my_view);

    for (auto i = test_world.getChunksBegin(); i != test_world.getChunksEnd(); i++)
    {
        target.draw(i->second->getGroundVertexArray(), ground_textures);
        target.draw(i->second->getGroundDetailsVertexArray(anim_frame), ground_details_textures);
        target.draw(i->second->getBlockSidesVertexArray(), block_textures);
        target.draw(i->second->getBlockTopsVertexArray(), block_textures);
    }
}

void GameState::updateView()
{
    sf::RenderWindow& window = getGame()->getWindow();
    //Resize without deformation
	if (window.getSize().y > window.getSize().x)
	{
		float ratio = (float)window.getSize().y / window.getSize().x;
		float y_size = ratio * zoom;
		my_view.setSize(sf::Vector2f(zoom, y_size));
	}
	else
	{
		float ratio = (float)window.getSize().x / window.getSize().y;
		float x_size = ratio * zoom;
		my_view.setSize(sf::Vector2f(x_size, zoom));
	}
}
