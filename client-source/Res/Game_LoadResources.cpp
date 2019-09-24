#include "../Game.h"

#include <iostream>
#include <SFML/System.hpp>

void Game::loadResources()
{
    sf::Clock clk;

    uint32_t fail_count = 0;

    std::cout << "Loading all resources..." << std::endl;

    //TEXTURES
    //Terrain
    fail_count +=
        !resource_manager.loadTextureFromFile("Terrain/grounds.png", "GROUND_TEXTURES") +
        !resource_manager.loadTextureFromFile("Terrain/ground_details.png", "GROUND_DETAILS") +
        //!resource_manager.loadTextureFromFile("Testing/rotation_test.png", "ROTATION_TESTS") +
        !resource_manager.loadTextureFromFile("Terrain/blocks.png", "BLOCK_TEXTURES");
    //Entities
    fail_count +=
        !resource_manager.loadTextureFromFile("Testing/animation_test.png", "ANIM_TEST"),
        !resource_manager.loadTextureFromFile("Testing/character_3d_test.png", "CHARA_TEST");

    //Fonts
    fail_count +=
        !resource_manager.loadFontFromFile("Temp/Code New Roman.otf", "GUI_FONT");

    std::cout << "Resources loaded in " << clk.getElapsedTime().asSeconds() << "s." << std::endl;
    if (fail_count)
        std::cout << fail_count << " resources could not be loaded." << std::endl;
}
