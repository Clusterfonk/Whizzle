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

#ifndef WAITING_BAR_HEADER
#define WAITING_BAR_HEADER

#include <SFML/Graphics.hpp>

#include "label.h"
#include "manager/resource_manager.h"


class Waiting_bar : public Label {
    public:
        Waiting_bar(sf::Vector2f origin);
        ~Waiting_bar();

        enum class type { 
            WAITING,
            FULL, 
            READY,
            NONE
        };

        void draw(sf::RenderTarget& target);
        void update(const sf::Time& delta_time);
        void set_type(type bar_type);

        void is_playable();

    private:
        sf::Texture bar_tex;
        sf::Sprite bar_sprite;
        Label bar_label;
        type crt_type = type::NONE;
        bool playable = false;
};
#endif
