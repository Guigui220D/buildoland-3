#pragma once

#include <functional>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "GuiElement.h"

class TextInput : public GuiElement
{
public:
    TextInput(Game& game, sf::Vector2f pos, float width,  std::string placeHolder="", unsigned maxSize = 0, bool alphaNumeric = false);

    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override ;
    bool handleEvent(sf::Event& e) override;

    void setInputText(const sf::String& str);
    const sf::String &getInputText() const;

    inline void setValidator(const std::function<bool(const sf::String&)>& callback)
    {
        validator = callback;
    }
    inline bool valid() const
    {
        if (!validator)
            return true;
        else
            return validator(inputText);
    }

    inline void setActive(bool activ) { active = activ; }
    bool isActive() const { return active; }
    bool isStillPlaceHolder() const { return isPlaceHolder; }

    bool onEnter();

private:
    const float margin = 15.0f;
    float cursorTime = 0.0f;
    sf::String inputText;
    bool isPlaceHolder = true;
    unsigned maxSize;
    bool active = false;
    sf::Vector2f position;

    std::function<bool(const sf::String&)> validator;

    sf::RectangleShape background;
    sf::RectangleShape cursor;
    sf::Text text;

    void eventInput(sf::Uint32 unicode);
    bool alphaNumericOnly;

    bool enterPressed;
};



