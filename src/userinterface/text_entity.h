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

#ifndef TEXT_ENTITY_HEADER
#define TEXT_ENTITY_HEADER

#include <SFML/Graphics.hpp>
#include <string.h>

#include "manager/resource_manager.h"


class Text_entity {
    public:
        Text_entity();  
        Text_entity(sf::Vector2f pos, unsigned int size, 
                bool should_resize, unsigned int max_size);  
        ~Text_entity();

        void update(const sf::Time& delta_time);
        void draw(sf::RenderTarget& target);

        bool add_char(char in_char);
        void rm_char();
        void clear();

        void set_position(sf::Vector2f pos);
        void resize();
        void set_color(int r, int g, int b, int a);

        bool is_active();
        void set_active(bool set_val);
        void set_max_size(unsigned new_size);

        std::string get_text();

        int alpha_channel = 255;
    protected:
        sf::Vector2f origin;
        sf::Text text;

        bool active = false;
        bool resizing = false;

        float indicator_timer = 0.7f; // corrects first interval
        float blink_timer = indicator_timer;
        const char indicator = '|';
        bool indication = true;
        void rm_indicator();

        unsigned int MAX_STRING_SIZE;
};
#endif
