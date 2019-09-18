#include "../Game.h"

#include <iostream>
#include <SFML/System.hpp>

void Game::loadResources()
{
    sf::Clock clk;

    uint32_t fail_count = 0;

    std::cout << "Loading all resources..." << std::endl;

    fail_count +=
        !resourceManager.loadTextureFromFile("Terrain/grounds.png", "GROUND_TEXTURES") +
        !resourceManager.loadTextureFromFile("Terrain/ground_details.png", "GROUND_DETAILS") +
        //!resourceManager.loadTextureFromFile("Terrain/rotation_test.png", "ROTATION_TESTS") +
        !resourceManager.loadTextureFromFile("Terrain/blocks.png", "BLOCK_TEXTURES");

    std::cout << "Resources loaded in " << clk.getElapsedTime().asSeconds() << "s." << std::endl;
    if (fail_count)
        std::cout << fail_count << " resources could not be loaded." << std::endl;
}
