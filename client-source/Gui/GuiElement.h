#pragma once

namespace sf
{
class Event;
class RenderTarget;
}

class Game;

class GuiElement
{
    public:
        GuiElement(Game& game);
        virtual ~GuiElement();

        virtual void init();
        virtual bool handleEvent(sf::Event& event);
        virtual void draw(sf::RenderTarget& target) const;
        virtual void update(float delta_time);

        inline Game& getGame() const { return game; }

    protected:
        Game& game;

    private:
};
