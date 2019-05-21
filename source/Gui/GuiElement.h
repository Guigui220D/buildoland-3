#pragma once

#include <SFML/Graphics.hpp>

//Used to know where to place a gui in its zone
enum GuiAlign
{
    TopOrLeft = -1,
    Center = 0,
    BottomOrRight = 1
};

class Game;

class GuiElement
{
    public:
        /**
         * Base constructor for a Gui element
         * @param zone : The place where the element should be fit on the screen (values between 0 and 1)
         * @param aspect_ratio : the ratio of the element (not of the zone), it's the width divided by the height
         */
        GuiElement(Game* game, sf::FloatRect zone, float aspect_ratio, GuiAlign horizontal_align, GuiAlign vertical_align);
        virtual ~GuiElement();

        /**
         * Takes an event and handle it, or not.
         * @param event : the even to handle
         * @return True if the event was caught by the gui element
         */
        virtual bool handleEvent(sf::Event& event);
        /**
         * Draws the element on the render target
         * Should call protected function useView which changes the window's view
         * @param target : where the element needs to be drawn
         */
        virtual void draw(sf::RenderTarget& target) const;
        /**
         * Draws the zone to see where it is and if everything works
         * @param target : where the debug rectangles needs to be drawn
         */
        void debugDraw(sf::RenderTarget& target) const;
        /**
         * Does a tick to update the gui element
         * @param delta_time : the seconds that have passed since the last update
         */
        virtual void update(float delta_time);

        /**
         * Calculates the view, must be called every time the window is resized or when the element is created
         * @param window_size : The size of the window in pixels
         */
        void calculateView(sf::Vector2u window_size);

        /**
         * Gets the zone dedicated to the element
         * @return The zone dedicated to the element
         */
        inline sf::FloatRect getZone() const { return zone; }
        /**
         * Gets the aspect ratio (width / height of the object)
         * @return The aspect ratio
         */
        inline float getRatio() const { return ratio; }
        /**
         * Gets the horizontal align
         * @return The horizontal align
         */
        inline GuiAlign getHorizontalAlign() const { return halign; }
        /**
         * Gets the vertical align
         * @return The vertical align
         */
        inline GuiAlign getValignAlign() const { return halign; }
        /**
         * To get the the game this element belongs to
         * @return The pointer to the game
         */
        inline Game* getGame() const { return game; }

    protected:
        /**
         * Sets the view to the calculated view, please use when drawing
         */
        inline void useView(sf::RenderTarget& target) const { target.setView(calculated_view); }
        /**
         * Gets the calculated view
         * @return The view
         */
        inline sf::View getView() const { return calculated_view; }
        /**
         * Gets the aspect ratio
         * @return The ratio
         */
        inline float getAspectRatio() const { return ratio; }

    private:
        Game* game;

        sf::FloatRect zone;
        float ratio;
        GuiAlign halign, valign;
        sf::View calculated_view;
};
