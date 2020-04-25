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

    inline void setPosition(float x, float y)
    {
        position = {x, y};
        background.setPosition(x, y);
        text.setPosition(x + 5.f, y + 2.5f);
        cursor.setPosition(x + text.getGlobalBounds().width + 6.5f, y + background.getSize().y/2.f);
    }
    inline void setSize(const sf::Vector2f& size)
    {
        background.setSize(size);
        cursor.setSize(sf::Vector2f(2, size.y*0.65f));
        cursor.setOrigin(cursor.getSize()/2.f);
        cursor.setPosition(position.x + text.getGlobalBounds().width + 6.5f, position.y + size.y/2.f);
    }
    inline void setCharacterSize(unsigned size)
    {
        text.setCharacterSize(size);
    }

    void clear();

    inline void setValidator(const std::function<bool(const sf::String&)>& callback)
    {
        validator = callback;
    }
    inline void setEnterCallback(const std::function<void(const sf::String&)>& callback)
    {
        enter_callback = callback;
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

private:
    const float margin = 15.0f;
    float cursorTime = 0.0f;
    sf::String inputText;
    bool isPlaceHolder = true;
    unsigned maxSize;
    bool active = false;
    sf::Vector2f position;

    std::function<bool(const sf::String&)> validator;
    std::function<void(const sf::String&)> enter_callback;

    sf::RectangleShape background;
    sf::RectangleShape cursor;
    sf::Text text;

    void eventInput(sf::Uint32 unicode);
    bool alphaNumericOnly;

    bool enterPressed;
};



