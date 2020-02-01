/**
 * This file is part of Whizzle.
 * Copyright (c) 2019 Till Vogel
 *
 *  Whizzle is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Whizzle is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Whizzle.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BUTTON_BAR_HEADER
#define BUTTON_BAR_HEADER

#include <SFML/Graphics.hpp>

#include "label.h"
#include "manager/input_manager.h"
#include "manager/resource_manager.h"

enum class Button_type {CREATE_ROOM, CONNECT, QUIT, BACK, NORMAL, INVERSE
};

class Button_bar {
    public:
        Button_bar(sf::Vector2f origin, sf::Vector2f size, const sf::String& txt,
                Button_type left, Button_type right, float offset);
        ~Button_bar();

        void draw(sf::RenderTarget& target);
        void update(sf::Window& window);
        bool is_clicked();

    private:
        sf::RectangleShape hitbox;

        sf::IntRect bar_frame;
        sf::Texture bar_tex;
        sf::Sprite bar_sprite;

        sf::Sprite left_icon;
        sf::Sprite right_icon;

        sf::IntRect calc_rect(Button_type b_t);

        sf::Clock anim_clock;
        sf::Time anim_time;

        Label bar_label;

};
#endif

