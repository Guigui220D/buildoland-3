#include "../Game.h"

#include <SFML/System/Clock.hpp>

#include "../../common-source/Utils/Log.h"

#include "GamePimpl.h"

void Game::loadResources()
{
    sf::Clock clk;

    uint32_t fail_count = 0;

    log(INFO, "Loading all resources...\n");

    //TEXTURES
    //Terrain
    fail_count +=
        !pimpl->resource_manager.loadTextureFromFile("Terrain/grounds.png", "GROUND_TEXTURES") +
        !pimpl->resource_manager.loadTextureFromFile("Terrain/ground_details.png", "GROUND_DETAILS") +
        //!pimpl->resource_manager.loadTextureFromFile("Testing/rotation_test.png", "ROTATION_TESTS") +
        !pimpl->resource_manager.loadTextureFromFile("Terrain/blocks.png", "BLOCK_TEXTURES");
    //Entities
    fail_count +=
        !pimpl->resource_manager.loadTextureFromFile("Temp/zombie.png", "ZOMBIE") +
        !pimpl->resource_manager.loadTextureFromFile("Character/naked.png", "CHARA_BASE") +
        !pimpl->resource_manager.loadTextureFromFile("Character/shoes.png", "CHARA_SHOES") +
        !pimpl->resource_manager.loadTextureFromFile("Character/pants.png", "CHARA_PANTS") +
        !pimpl->resource_manager.loadTextureFromFile("Character/shirt.png", "CHARA_SHIRT") +
        !pimpl->resource_manager.loadTextureFromFile("Entities/tree.png", "TREE_TOP");
    //Items
    fail_count +=
        !pimpl->resource_manager.loadTextureFromFile("Items/items.png", "ITEM_TEXTURES");
    //Gui
    fail_count +=
        !pimpl->resource_manager.loadTextureFromFile("Gui/inventory.png", "INVENTORY") +
        !pimpl->resource_manager.loadTextureFromFile("Gui/blocks_pointer.png", "BLOCK_POINTER") +
        !pimpl->resource_manager.loadTextureFromFile("Gui/blocks_pointer_rotate.png", "BLOCK_POINTER_B");
    //FONTS
    fail_count +=
        !pimpl->resource_manager.loadFontFromFile("Fonts/MONOFONT.TTF", "GUI_FONT");

    log(INFO, "Resources loaded in {}s.\n", clk.getElapsedTime().asSeconds());
    if (fail_count)
        log(INFO, "{} resources could not be loaded.\n", fail_count);
}

void Game::titleScreenInit()
{
    pimpl->language_manager.load(pimpl->settings_manager.getString("game_language"));
    loadResources();
    pimpl->game_blocks_manager.initBlocks();
    pimpl->game_grounds_manager.initGrounds();
    pimpl->game_items_register.initItems(pimpl->game_blocks_manager, pimpl->game_grounds_manager);

}
