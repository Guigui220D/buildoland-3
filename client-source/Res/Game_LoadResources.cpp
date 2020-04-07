#include "../Game.h"

#include <SFML/System.hpp>

#include "../../common-source/Utils/Log.h"

void Game::loadResources()
{
    sf::Clock clk;

    uint32_t fail_count = 0;

    log(INFO, "Loading all resources...\n");

    //TEXTURES
    //Terrain
    fail_count +=
        !resource_manager.loadTextureFromFile("Terrain/grounds.png", "GROUND_TEXTURES") +
        !resource_manager.loadTextureFromFile("Terrain/ground_details.png", "GROUND_DETAILS") +
        //!resource_manager.loadTextureFromFile("Testing/rotation_test.png", "ROTATION_TESTS") +
        !resource_manager.loadTextureFromFile("Terrain/blocks.png", "BLOCK_TEXTURES");
    //Entities
    fail_count +=
        !resource_manager.loadTextureFromFile("Temp/zombie.png", "ZOMBIE") +
        !resource_manager.loadTextureFromFile("Character/naked.png", "CHARA_BASE") +
        !resource_manager.loadTextureFromFile("Character/shoes.png", "CHARA_SHOES") +
        !resource_manager.loadTextureFromFile("Character/pants.png", "CHARA_PANTS") +
        !resource_manager.loadTextureFromFile("Character/shirt.png", "CHARA_SHIRT") +
        !resource_manager.loadTextureFromFile("Entities/tree.png", "TREE_TOP");
    //Items
    fail_count +=
        !resource_manager.loadTextureFromFile("Items/items.png", "ITEM_TEXTURES");
    //Gui
    fail_count +=
        !resource_manager.loadTextureFromFile("Gui/inventory.png", "INVENTORY") +
        !resource_manager.loadTextureFromFile("Gui/blocks_pointer.png", "BLOCK_POINTER") +
        !resource_manager.loadTextureFromFile("Gui/blocks_pointer_rotate.png", "BLOCK_POINTER_B");
    //FONTS
    fail_count +=
        !resource_manager.loadFontFromFile("Fonts/MONOFONT.TTF", "GUI_FONT");

    log(INFO, "Resources loaded in {}s.\n", clk.getElapsedTime().asSeconds());
    if (fail_count)
        log(INFO, "{} resources could not be loaded.\n", fail_count);
}
