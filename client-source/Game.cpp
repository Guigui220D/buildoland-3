#include "Game.h"

#include <cassert>

#include "States/TitleScreenState.h"

#include "GamePimpl.h"

Game::Game()
{
    pimpl = std::make_unique<GameImpl>(*this);
}

Game::~Game()
{
    //dtor
}

int Game::init()
{
    pimpl->settings_manager.load();
    pimpl->bindings_manager.load();

    pimpl->window.create(sf::VideoMode(pimpl->settings_manager.getInt("window_size_x"), pimpl->settings_manager.getInt("window_size_y")), "BuildOLand 3");

    pimpl->bg_zone.calculateView(pimpl->window.getSize());

    bool vsync = pimpl->settings_manager.getBool("vsync_enabled");

    if (!vsync)
    {
        int fps_limit = pimpl->settings_manager.getInt("fps_limit");
        if (fps_limit < 0)
            fps_limit = 0;
        pimpl->window.setFramerateLimit(fps_limit);
    }
    else
        pimpl->window.setVerticalSyncEnabled(true);

    addStateOnTop(new TitleScreenState(*this, 0));

    pimpl->bg_image->init();
    pimpl->bg_zone.addElement(pimpl->bg_image);
    return 0;
}

int Game::run()
{
    sf::Clock clock;

    sf::Clock fps_clk;
	int count = 0;

	sf::Clock dt_clk;
	sf::RectangleShape dt_limit(sf::Vector2f(85.f, (1.f / 60.f) * 1000.f));
	sf::RectangleShape dt_viewer(sf::Vector2f(1.f, 1.f));
	dt_viewer.setFillColor(sf::Color::Red);
	dt_limit.setFillColor(sf::Color::Green);

    while (pimpl->window.isOpen())
    {

        sf::Event e;
        while (pimpl->window.pollEvent(e))
        {
            pimpl->bindings_manager.update(e);

            switch (e.type)
            {
            case sf::Event::Closed:
                pimpl->window.close();
                break;
            case sf::Event::Resized:
                pimpl->bg_zone.calculateView(sf::Vector2u(e.size.width, e.size.height));
            default:
                {
                    for (int i = pimpl->states_stack.size() - 1; i >= 0; i--)
                    {
                        std::unique_ptr<State>& state = pimpl->states_stack.at(i);
                        bool caught = state->handleEvent(e);
                        if (caught)
                            break;
                    }
                }
                break;
            }

        }

        float s = clock.restart().asSeconds();

        if (s >= .02f)
            log(INFO, "Spikelag of : {}s\n", s);

        update(s);

        //FPS VIEWER
        dt_viewer.setSize(sf::Vector2f(80.f, dt_clk.restart().asMilliseconds()));

        pimpl->window.clear();

        //bg_zone.draw(window);
        //FPS VIEWER //TEMP
        draw();

        pimpl->window.setView(pimpl->window.getDefaultView());
        pimpl->window.draw(dt_limit);
        pimpl->window.draw(dt_viewer);

        pimpl->window.display();

        //Remove states
        for (auto i = pimpl->states_stack.begin(); i != pimpl->states_stack.end(); )
        {
            if ((*i)->must_be_destroyed)
            {
                (*i).reset();
                pimpl->states_stack.erase(i);
            }
            else i++;
        }
        //Add states on top
        for (State*& state : pimpl->states_to_add_on_top)
            pimpl->states_stack.push_back(std::unique_ptr<State>(state));
        pimpl->states_to_add_on_top.clear();
        //Add states under the top
        if (pimpl->state_to_add_under_the_top)
        {
            assert(pimpl->states_stack.size());
            pimpl->states_stack.insert(pimpl->states_stack.end() - 1, std::unique_ptr<State>(pimpl->state_to_add_under_the_top));
            pimpl->state_to_add_under_the_top = nullptr;
        }

        count++;
		if (count >= 1000)
		{
			float time = fps_clk.restart().asSeconds();
			time /= count;
            float fps = 1 / time;
            log(INFO, "FPS : {}\n", fps);
			count = 0;
		}
    }
    return 0;
}

void Game::exit()
{

}


void Game::addStateOnTop(State* state, bool init)
{
    if (init)
        state->init();
    pimpl->states_to_add_on_top.push_back(state);
}

void Game::addStateUnderTop(State* state, bool init)
{
    assert(!pimpl->state_to_add_under_the_top);
    if (init)
        state->init();
    pimpl->state_to_add_under_the_top = state;
}

sf::RenderWindow &Game::getWindow() { return pimpl->window; }

const GameBlocks &Game::getBlocksManager() const { return pimpl->game_blocks_manager; }

const GameGrounds &Game::getGroundsManager() const { return pimpl->game_grounds_manager; }

const ItemsRegister &Game::getItemsRegister() const { return pimpl->game_items_register; }

const ResourceManager &Game::getResourceManager() const { return pimpl->resource_manager; }

AudioManager &Game::getAudioManager() { return pimpl->audio_manager; }

SettingsManager &Game::getSettingsManager() { return pimpl->settings_manager; }

LanguageManager &Game::getLanguageManager() { return pimpl->language_manager; }

BindingsManager &Game::getBindingsManager() { return pimpl->bindings_manager; }

void Game::useDefaultView() { pimpl->window.setView(pimpl->default_view); }

State *Game::getTopState() const { return pimpl->states_stack.back().get(); }


void Game::draw()
{
    if (pimpl->states_stack.empty())
        return;

    unsigned int i;
    for (i = pimpl->states_stack.size() - 1; i > 0; i--)
    {
        if (!pimpl->states_stack.at(i)->isDrawTransparent())
            break;
    }
    for (; i < pimpl->states_stack.size(); i++)
        pimpl->states_stack.at(i)->draw(pimpl->window);
}

void Game::update(float delta_time)
{
    for (int i = pimpl->states_stack.size() - 1; i >= 0; i--)
    {
        std::unique_ptr<State>& state = pimpl->states_stack.at(i);
        state->update(delta_time);
        if (!state->isUpdateTransparent())
            break;
    }

    pimpl->audio_manager.update();
}
