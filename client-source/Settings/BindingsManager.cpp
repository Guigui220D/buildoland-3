#include "BindingsManager.h"

#include <SFML/System/Clock.hpp>

#include <fstream>

#include "../../external/json/Json.hpp"

#include "../../common-source/Utils/Log.h"

const std::string BindingsManager::BINDINGS_FILE_PATH = "Resources/Settings/bindings.json";

BindingsManager::BindingsManager()
{
    json = std::make_unique<nlohmann::json>();
}

BindingsManager::~BindingsManager()
{
}

std::string strtolower(std::string str)
{
    for (auto& c : str)
        c = tolower(c);

    return str;
}

void BindingsManager::update(const sf::Event &event)
{
    for (auto& pair : bindings)
    {
        for (auto& binding : pair.second)
        {
            binding.pressed = false; // reset

            if (binding.type == Binding::Keyboard)
            {
                binding.active = sf::Keyboard::isKeyPressed(binding.key);
                if (event.type == sf::Event::KeyPressed && event.key.code == binding.key)
                    binding.pressed = true;
            }
            else if (binding.type == Binding::Mouse)
            {
                binding.active = sf::Mouse::isButtonPressed(binding.mouse_button);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == binding.mouse_button)
                    binding.pressed = true;
            }
            else if (binding.type == Binding::JoystickButton)
            {
                binding.active = sf::Joystick::isButtonPressed(binding.joy_button, 0); // we support only one joystick
                if (event.type == sf::Event::JoystickButtonPressed && (int)event.joystickButton.button == binding.joy_button)
                    binding.pressed = true;
            }
            else if (binding.type == Binding::JoystickAxis)
            {
                float axis_pos = sf::Joystick::getAxisPosition(0, binding.joy_axis.axis);
                const float thresold = 10;

                if (binding.joy_axis.high)
                {
                    binding.active = axis_pos >= thresold;
                    if (event.type == sf::Event::JoystickMoved
                        && event.joystickMove.axis == binding.joy_axis.axis
                        && event.joystickMove.position >= thresold)
                        binding.pressed = true;
                }
                else
                {
                    binding.active = axis_pos < -thresold;
                    if (event.type == sf::Event::JoystickMoved
                        && event.joystickMove.axis == binding.joy_axis.axis
                        && event.joystickMove.position < -thresold)
                        binding.pressed = true;
                }
            }
            else
                assert("unknown binding type" && false);
        }
    }
}

bool BindingsManager::pressed(const std::string &binding) const
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

bool BindingsManager::held(const std::string &binding) const
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

void BindingsManager::load()
{
    std::ifstream is(BINDINGS_FILE_PATH);
    if (!is.is_open())
    {
        log(ERROR, "Could not load bindings file!\n");
        *json = "{}"_json;
    }
    else
        is >> *json;

    sf::Clock clk;

    log(INFO, "Started loading bindings...\n");

    for (const auto& node : json->items())
    {
        if (node.value().is_array())
        {
            for (const auto& elem : node.value())
                addBinding(node.key(), elem);
        }
        else if (node.value().is_string())
        {
            addBinding(node.key(), node.value());
        }
    }

    log(INFO, "Loaded all bindings in {}s.\n", clk.getElapsedTime().asSeconds());
}

void BindingsManager::addBinding(const std::string &key, const std::string &val)
{
    Binding binding;
    std::string value = strtolower(val);

    JoyAxisMapping val_axis = getAxis(value);
    int val_joy_button = getJoyButton(value);
    sf::Mouse::Button val_mouse = getMouse(value);
    sf::Keyboard::Key val_key = getKey(value);
    if (val_axis.axis != (sf::Joystick::Axis)-1)
    {
        binding.type = Binding::JoystickAxis;
        binding.joy_axis = val_axis;
        bindings[key].push_back(binding);
    }
    else if (val_joy_button != -1)
    {
        binding.type = Binding::JoystickButton;
        binding.joy_button = val_joy_button;
        bindings[key].push_back(binding);
    }
    else if (val_mouse != (sf::Mouse::Button)-1)
    {
        binding.type = Binding::Mouse;
        binding.mouse_button = val_mouse;
        bindings[key].push_back(binding);
    }
    else if (val_key != -1)
    {
        binding.type = Binding::Keyboard;
        binding.key = val_key;
        bindings[key].push_back(binding);
    }
    else
    {
        log(ERROR, "Unknown input : '{}'\n", value);
    }
}

sf::Keyboard::Key BindingsManager::getKey(const std::string &str)
{
#define ENTRY(val) if (str == strtolower(#val)) return sf::Keyboard::val;
    ENTRY(A);        ///< The A key
    ENTRY(B);            ///< The B key
    ENTRY(C);            ///< The C key
    ENTRY(D);            ///< The D key
    ENTRY(E);            ///< The E key
    ENTRY(F);            ///< The F key
    ENTRY(G);            ///< The G key
    ENTRY(H);            ///< The H key
    ENTRY(I);            ///< The I key
    ENTRY(J);            ///< The J key
    ENTRY(K);            ///< The K key
    ENTRY(L);            ///< The L key
    ENTRY(M);            ///< The M key
    ENTRY(N);            ///< The N key
    ENTRY(O);            ///< The O key
    ENTRY(P);            ///< The P key
    ENTRY(Q);            ///< The Q key
    ENTRY(R);            ///< The R key
    ENTRY(S);            ///< The S key
    ENTRY(T);            ///< The T key
    ENTRY(U);            ///< The U key
    ENTRY(V);            ///< The V key
    ENTRY(W);            ///< The W key
    ENTRY(X);            ///< The X key
    ENTRY(Y);            ///< The Y key
    ENTRY(Z);            ///< The Z key
    ENTRY(Num0);         ///< The 0 key
    ENTRY(Num1);         ///< The 1 key
    ENTRY(Num2);         ///< The 2 key
    ENTRY(Num3);         ///< The 3 key
    ENTRY(Num4);         ///< The 4 key
    ENTRY(Num5);         ///< The 5 key
    ENTRY(Num6);         ///< The 6 key
    ENTRY(Num7);         ///< The 7 key
    ENTRY(Num8);         ///< The 8 key
    ENTRY(Num9);         ///< The 9 key
    ENTRY(Escape);       ///< The Escape key
    ENTRY(LControl);     ///< The left Control key
    ENTRY(LShift);       ///< The left Shift key
    ENTRY(LAlt);         ///< The left Alt key
    ENTRY(LSystem);      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
    ENTRY(RControl);     ///< The right Control key
    ENTRY(RShift);       ///< The right Shift key
    ENTRY(RAlt);         ///< The right Alt key
    ENTRY(RSystem);      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
    ENTRY(Menu);         ///< The Menu key
    ENTRY(LBracket);     ///< The [ key
    ENTRY(RBracket);     ///< The ] key
    ENTRY(Semicolon);    ///< The ; key
    ENTRY(Comma);        ///< The , key
    ENTRY(Period);       ///< The . key
    ENTRY(Quote);        ///< The ' key
    ENTRY(Slash);        ///< The / key
    ENTRY(Backslash);    ///< The \ key
    ENTRY(Tilde);        ///< The ~ key
    ENTRY(Equal);        ///< The = key
    ENTRY(Hyphen);       ///< The - key (hyphen)
    ENTRY(Space);        ///< The Space key
    ENTRY(Enter);        ///< The Enter/Return keys
    ENTRY(Backspace);    ///< The Backspace key
    ENTRY(Tab);          ///< The Tabulation key
    ENTRY(PageUp);       ///< The Page up key
    ENTRY(PageDown);     ///< The Page down key
    ENTRY(End);          ///< The End key
    ENTRY(Home);         ///< The Home key
    ENTRY(Insert);       ///< The Insert key
    ENTRY(Delete);       ///< The Delete key
    ENTRY(Add);          ///< The + key
    ENTRY(Subtract);     ///< The - key (ENTRY(minus); usually from numpad)
    ENTRY(Multiply);     ///< The * key
    ENTRY(Divide);       ///< The / key
    ENTRY(Left);         ///< Left arrow
    ENTRY(Right);        ///< Right arrow
    ENTRY(Up);           ///< Up arrow
    ENTRY(Down);         ///< Down arrow
    ENTRY(Numpad0);      ///< The numpad 0 key
    ENTRY(Numpad1);      ///< The numpad 1 key
    ENTRY(Numpad2);      ///< The numpad 2 key
    ENTRY(Numpad3);      ///< The numpad 3 key
    ENTRY(Numpad4);      ///< The numpad 4 key
    ENTRY(Numpad5);      ///< The numpad 5 key
    ENTRY(Numpad6);      ///< The numpad 6 key
    ENTRY(Numpad7);      ///< The numpad 7 key
    ENTRY(Numpad8);      ///< The numpad 8 key
    ENTRY(Numpad9);      ///< The numpad 9 key
    ENTRY(F1);           ///< The F1 key
    ENTRY(F2);           ///< The F2 key
    ENTRY(F3);           ///< The F3 key
    ENTRY(F4);           ///< The F4 key
    ENTRY(F5);           ///< The F5 key
    ENTRY(F6);           ///< The F6 key
    ENTRY(F7);           ///< The F7 key
    ENTRY(F8);           ///< The F8 key
    ENTRY(F9);           ///< The F9 key
    ENTRY(F10);          ///< The F10 key
    ENTRY(F11);          ///< The F11 key
    ENTRY(F12);          ///< The F12 key
    ENTRY(F13);          ///< The F13 key
    ENTRY(F14);          ///< The F14 key
    ENTRY(F15);          ///< The F15 key
    ENTRY(Pause);        ///< The Pause key
#undef ENTRY

    return (sf::Keyboard::Key)-1;
}

sf::Mouse::Button BindingsManager::getMouse(const std::string &str)
{
    if (str == "mouseleft")
        return sf::Mouse::Left;
    if (str == "mouseright")
        return sf::Mouse::Right;
    if (str == "mousemiddle")
        return sf::Mouse::Middle;
    if (str == "mousxbutton1")
        return sf::Mouse::XButton1;
    if (str == "mousxbutton2")
        return sf::Mouse::XButton2;

    return (sf::Mouse::Button)-1;
}

BindingsManager::JoyAxisMapping BindingsManager::getAxis(const std::string &str)
{
    JoyAxisMapping mapping;

#define ENTRY(val) if (str == strtolower("joy_" #val "_high")){ \
    mapping.axis = sf::Joystick::val; \
    mapping.high = true; \
    return mapping;} \
    else if (str == strtolower("joy_" #val "_low")){ \
    mapping.axis = sf::Joystick::val; \
    mapping.high = false; \
    return mapping;} \


    ENTRY(X);
    ENTRY(Y);
    ENTRY(Z);
    ENTRY(R);
    ENTRY(U);
    ENTRY(V);
    ENTRY(PovX);
    ENTRY(PovY);
#undef ENTRY

    mapping.axis = (sf::Joystick::Axis)-1;
    return mapping;
}

int BindingsManager::getJoyButton(const std::string &str)
{
    if (str.substr(0, 4) != "joy_")
        return -1;

    int button = std::stoi(str.substr(4));

    if (button >= sf::Joystick::ButtonCount)
        return -1;

    return button;
}
