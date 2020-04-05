#pragma once

#include <string>
#include <initializer_list>
#include <unordered_map>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include "../../external/json/Json.hpp"

//This class can load and save bindings
class BindingsManager
{
    friend class Game;

public:
    void update(const sf::Event& event);

    bool pressed(const std::string& binding) const
    {
        auto i = bindings.find(binding);
        if (i == bindings.end())
            throw new std::out_of_range("Binding \"" + binding + "\" doesn't exist.");

        for (const auto& binding : i->second)
        {
            if (binding.pressed)
                return true;
        }
        return false;
    }
    bool held(const std::string& binding) const
    {
        auto i = bindings.find(binding);
        if (i == bindings.end())
            throw new std::out_of_range("Binding \"" + binding + "\" doesn't exist.");

        for (const auto& binding : i->second)
        {
            if (binding.active)
                return true;
        }
        return false;
    }

private:
    static const std::string BINDINGS_FILE_PATH;

    struct JoyAxisMapping
    {
        sf::Joystick::Axis axis;
        bool high;
    };

    struct Binding
    {
        enum
        {
            Keyboard,
            JoystickAxis,
            JoystickButton,
            Mouse
        } type;
        union
        {
            sf::Keyboard::Key key;
            sf::Mouse::Button mouse_button;
            JoyAxisMapping joy_axis;
            int joy_button;
        };

        int pressed { false };
        int active  { false };
    };

    using BindingList = std::vector<Binding>;

    BindingsManager();
    ~BindingsManager();

    void load();

    void addBinding(const std::string& key, const std::string& val);

    sf::Keyboard::Key getKey(const std::string& str);
    sf::Mouse::Button getMouse(const std::string& str);
    JoyAxisMapping getAxis(const std::string& str);
    int getJoyButton(const std::string& str);

    std::unordered_map<std::string, BindingList> bindings;

    nlohmann::json json;
};
