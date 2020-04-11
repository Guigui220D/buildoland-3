#pragma once

#include "GuiElement.h"

#include "../Game.h"

#include <string>

#include "../../client-source/Settings/LanguageManager.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>

class GuiLabel : public GuiElement
{
    public:
        template <typename... FmtArgs>
        GuiLabel(Game& game, sf::Vector2f position, const std::string& message, FmtArgs&&... fmt_args) :
              GuiElement(game),
              message(message)
        {
            setMessage(std::forward<FmtArgs>(fmt_args)...);
            text.setPosition(position);
        }
        ~GuiLabel();

        void draw(sf::RenderTarget& target) const override;
        void init() override;

        template <typename... FmtArgs>
        void setMessage(FmtArgs&&... args)
        {
            text.setString(game.getLanguageManager().getString(message, std::forward<FmtArgs>(args)...));
        }

    private:
        sf::Text text;
        const std::string message;
};
