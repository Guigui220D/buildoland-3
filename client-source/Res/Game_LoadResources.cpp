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
        !resource_manager.loadTextureFromFile("Testing/animation_test.png", "ANIM_TEST") +
        !resource_manager.loadTextureFromFile("Temp/test_total.png", "CHARA_TEST");
    //Gui
    fail_count +=
        !resource_manager.loadTextureFromFile("Gui/blocks_pointer.png", "BLOCK_POINTER") +
        !resource_manager.loadTextureFromFile("Gui/blocks_pointer_rotate.png", "BLOCK_POINTER_B");
    //Fonts
    fail_count +=
        !resource_manager.loadFontFromFile("Fonts/MONOFONT.TTF", "GUI_FONT");

    std::cout << "Resources loaded in " << clk.getElapsedTime().asSeconds() << "s." << std::endl;
    if (fail_count)
        std::cout << fail_count << " resources could not be loaded." << std::endl;
}
