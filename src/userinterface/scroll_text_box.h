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

#ifndef SCROLL_TEXT_BOX_HEADER
#define SCROLL_TEXT_BOX_HEADER

#include <SFML/Graphics.hpp>
#include <vector>

#include "manager/resource_manager.h"


class Scroll_text_box {
public:
    Scroll_text_box(sf::Vector2f pos, sf::Vector2f window_size);

    void update(const sf::Time& delta_time);
    void draw(sf::RenderTarget& target);

    void add_string(const std::string& txt);
    void scroll(int direction);

private:
    //icon for scrollbar
    //icon for reaching bottom

    std::vector<sf::Text> txt_buffer;
    unsigned int char_size;
    float scroll_offset;
    float txt_offset;
    int vis_txt_start;
    int vis_txt_end;
    int scroll_step;

    sf::Time up_threshold = sf::Time::Zero;
    sf::Time down_threshold = sf::Time::Zero;

    sf::Vector2f window_pos;
    sf::Vector2f window_size;
};
#endif
