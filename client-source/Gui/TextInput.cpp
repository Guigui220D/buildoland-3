#include "TextInput.h"

#include "../Game.h"
#include "../../client-source/Res/ResourceManager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Clipboard.hpp>

TextInput::TextInput(Game &game, sf::Vector2f pos, float width, std::string placeHolder, unsigned maxSize, bool alphaNumeric)
    : GuiElement(game),
      inputText(placeHolder),
      maxSize(maxSize),
      position(pos)
{
    alphaNumericOnly = alphaNumeric;

    //BACKGROUND
    background = sf::RectangleShape();
    background.setFillColor(sf::Color(100, 100, 100));
    background.setSize(sf::Vector2f(width, 100.f));
    background.setPosition(pos.x, pos.y);


    //TEXT
    text.setFont(getGame().getResourceManager().getFont("GUI_FONT"));
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(200);
    text.setScale(sf::Vector2f(.3f, .3f));
    text.setString(placeHolder);
    text.setPosition(pos.x + 5.f, pos.y + 2.5f);

    //CURSOR ( | )
    cursor = sf::RectangleShape();
    cursor.setFillColor(sf::Color::White);
    cursor.setSize(sf::Vector2f(2, 65));
    cursor.setOrigin(cursor.getSize()/2.f);
    cursor.setPosition(pos.x + text.getGlobalBounds().width + 6.5f, pos.y + 50.f);
}

void TextInput::update(float dt) {
    cursorTime += dt;
    //Reset after 2 secs
    cursorTime = cursorTime > 1.06f ? 0 : cursorTime;
}

void TextInput::draw(sf::RenderTarget &target) const
{
    target.draw(background);
    target.draw(text);
    if(active && cursorTime <= 0.53f) {
        target.draw(cursor);
    }
}

bool TextInput::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos = getGame().getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame().getWindow()));
        active = background.getGlobalBounds().contains(mousePos);
    }
    else if (active && e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::Return)
        {
            enterPressed = true;
            active = false;
            return true;
        }
        else if (e.key.code == sf::Keyboard::V && e.key.control)
        {
            sf::String new_string = inputText + sf::Clipboard::getString();
            setInputText(new_string);
            return true;
        }
        else if (e.key.code == sf::Keyboard::C && e.key.control)
        {
            sf::Clipboard::setString(inputText);
            return true;
        }
    }
    else if (e.type == sf::Event::TextEntered && active)
    {
        eventInput(e.text.unicode);
        return true;
    }
    return false;
}

void TextInput::setInputText(const sf::String &str)
{
    inputText = str;

    text.setString(inputText);
    cursor.setPosition(position.x + text.getGlobalBounds().width + 6.5f, position.y + 50.f);

    if (validator)
    {
        bool result = validator(inputText);
        if (!result)
        {
            text.setFillColor(sf::Color::Red);
        }
        else
        {
            text.setFillColor(sf::Color::White);
        }
    }
}

void TextInput::eventInput(sf::Uint32 unicode) {
    if(active) {
        //First time typing something
        if(isPlaceHolder) {
            isPlaceHolder = false;
            inputText = "";
        }
        //Erase key
        if(unicode == 8) {
            if(inputText.getSize() >= 1)
            {
                inputText = inputText.substring(0, inputText.getSize() - 1);
            }
        } else if(maxSize == 0 || inputText.getSize() < maxSize){
            if (!alphaNumericOnly
                || unicode == 45 || unicode == 95     //- and _
                || (unicode >= 97 && unicode <= 122)  //letters
                || (unicode >= 65 && unicode <= 90)   //LETTERS
                || (unicode >= 48 && unicode <= 57))
            {
                // if ASCII, check that it is a printable ascii character
                if (unicode <= 127 && !isprint(unicode))
                    return;

                inputText += sf::String(unicode);
            }
        }

        setInputText(inputText);
    }
}

const sf::String &TextInput::getInputText() const
{
    return inputText;
}

bool TextInput::onEnter()
{
    if (!enterPressed)
        return false;
    enterPressed = false;
    return true;
}
