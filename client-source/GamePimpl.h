#include <SFML/Graphics/RenderWindow.hpp>

#include "Settings/SettingsManager.h"
#include "Settings/LanguageManager.h"
#include "Settings/BindingsManager.h"
#include "Res/ResourceManager.h"
#include "Res/AudioManager.h"

#include "../common-source/Blocks/GameBlocks.h"
#include "../common-source/Grounds/GameGrounds.h"
#include "../common-source/Items/ItemsRegister.h"


#include "Gui/GuiZone.h"
#include "Gui/GuiImage.h"
#include "States/State.h"


struct GameImpl
{
    GameImpl(Game& game) :
        game_items_register(language_manager),
        audio_manager(resource_manager),
        bg_zone(game, sf::Vector2f(1.f, 1.f), GuiZone::Center, GuiZone::Top),
        bg_image(new GuiImage(game, sf::Vector2f(), sf::Vector2f(1000.f, 666.66f /*please don't summon demons*/), "Resources/Temp/background.png", false))
    {}

    sf::RenderWindow window;
    sf::View default_view;

    SettingsManager settings_manager;
    LanguageManager language_manager;
    BindingsManager bindings_manager;

    GameBlocks game_blocks_manager;
    GameGrounds game_grounds_manager;
    ItemsRegister game_items_register;

    ResourceManager resource_manager;
    AudioManager audio_manager;

    unsigned int framerate_target = 60;

    //All the states currently loaded
    std::vector<std::unique_ptr<State>> states_stack;
    std::vector<State*> states_to_add_on_top;
    State* state_to_add_under_the_top = nullptr; //Wow long name

    GuiZone bg_zone;
    GuiImage* bg_image;
};
