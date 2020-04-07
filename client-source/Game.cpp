#include "Game.h"

#include <assert.h>

#include "States/TitleScreenState.h"

Game::Game() :
    game_items_register(language_manager),
    audio_manager(resource_manager)
{

}

Game::~Game()
{
    //dtor
}

int Game::init()
{
    settings_manager.load();
    bindings_manager.load();

    window.create(sf::VideoMode(settings_manager.getInt("window_size_x"), settings_manager.getInt("window_size_y")), "BuildOLand 3");

    bool vsync = settings_manager.getBool("vsync_enabled");

    if (!vsync)
    {
        int fps_limit = settings_manager.getInt("fps_limit");
        if (fps_limit < 0)
            fps_limit = 0;
        window.setFramerateLimit(fps_limit);
    }
    else
        window.setVerticalSyncEnabled(true);

    addStateOnTop(new TitleScreenState(*this, 0));
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

    while (window.isOpen())
    {

        sf::Event e;
        while (window.pollEvent(e))
        {
            bindings_manager.update(e);

            switch (e.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                {
                    for (int i = states_stack.size() - 1; i >= 0; i--)
                    {
                        std::unique_ptr<State>& state = states_stack.at(i);
                        bool caught = state->handleEvent(e);
                        if (caught)
                            break;
                    }
                }
                break;
            }

        }

        update(clock.restart().asSeconds());

        //FPS VIEWER
        dt_viewer.setSize(sf::Vector2f(80.f, dt_clk.restart().asMilliseconds()));

        window.clear();
        //FPS VIEWER //TEMP
        draw();

        window.setView(window.getDefaultView());
        window.draw(dt_limit);
        window.draw(dt_viewer);

        window.display();

        //Remove states
        for (auto i = states_stack.begin(); i != states_stack.end(); )
        {
            if ((*i)->must_be_destroyed)
            {
                (*i).reset();
                states_stack.erase(i);
            }
            else i++;
        }
        //Add states on top
        for (State*& state : states_to_add_on_top)
            states_stack.push_back(std::unique_ptr<State>(state));
        states_to_add_on_top.clear();
        //Add states under the top
        if (state_to_add_under_the_top)
        {
            assert(states_stack.size());
            states_stack.insert(states_stack.end() - 1, std::unique_ptr<State>(state_to_add_under_the_top));
            state_to_add_under_the_top = nullptr;
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
    states_to_add_on_top.push_back(state);
}

void Game::addStateUnderTop(State* state, bool init)
{
    assert(!state_to_add_under_the_top);
    if (init)
        state->init();
    state_to_add_under_the_top = state;
}


void Game::draw()
{
    if (states_stack.empty())
        return;

    unsigned int i;
    for (i = states_stack.size() - 1; i > 0; i--)
    {
        if (!states_stack.at(i)->isDrawTransparent())
            break;
    }
    for (; i < states_stack.size(); i++)
        states_stack.at(i)->draw(window);
}

void Game::update(float delta_time)
{
    for (int i = states_stack.size() - 1; i >= 0; i--)
    {
        std::unique_ptr<State>& state = states_stack.at(i);
        state->update(delta_time);
        if (!state->isUpdateTransparent())
            break;
    }

    audio_manager.update();
}
