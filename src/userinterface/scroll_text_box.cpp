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

#include "scroll_text_box.h"

#define UP 1
#define DOWN -1

Scroll_text_box::Scroll_text_box(sf::Vector2f pos, sf::Vector2f window_size) {
    window_pos = pos;
    window_size = window_size;

    char_size = 20;
    scroll_offset = 0;
    vis_txt_start = 0;
    vis_txt_end = 0;
    scroll_step = 1;
}

/**
 * Draw max. 9 text lines starting from txt_vis_end to txt_vis_start
 * TODO: calc positions from window size, pos
 */
void Scroll_text_box::draw(sf::RenderTarget& target) {
    unsigned int row = 0;
    for(int i = vis_txt_end; i < vis_txt_start; i++) {
        txt_buffer[i].setPosition({10.0f, 400.0f + (row) * char_size});
        target.draw(txt_buffer[i]);
        row++;
    }
}

void Scroll_text_box::update(const sf::Time &delta_time) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if(up_threshold == sf::Time::Zero) {
            scroll(UP);
        }
        up_threshold += delta_time;
        if(up_threshold >= sf::seconds(0.1f)) {
            up_threshold = sf::Time::Zero;
        }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if(down_threshold == sf::Time::Zero) {
            scroll(DOWN);
        }
        down_threshold += delta_time;
        if(down_threshold >= sf::seconds(0.1f)) {
            down_threshold = sf::Time::Zero;
        }
    }

    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        up_threshold = sf::Time::Zero;
    }
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        down_threshold = sf::Time::Zero;
    }
}

/**
 * Add a line to the scroll_text_box.
 * Sets vis_txt_start to the latest msg and adjusts vis_txt_end, so that only 9 msgs in total can be displayed
 * When the text is too long to fit into one line another one will be created
 */
void Scroll_text_box::add_string(const std::string &txt) {
    sf::Text tmp = sf::Text(txt, Resource_manager::get_font("../assets/apercu_mono.ttf"), char_size);
    tmp.setFillColor(sf::Color::Black);
    if(tmp.getGlobalBounds().width > 395.0f) {
        sf::Text tmp = sf::Text("", Resource_manager::get_font("../assets/apercu_mono.ttf"), char_size);
        for(unsigned int i = 0; i < txt.size(); i++) {
            tmp.setString(txt.substr(0, i));
            if(tmp.getGlobalBounds().width >= 395.0f) {
                tmp.setString(txt.substr(0, i - 1));
                tmp.setFillColor(sf::Color::Black);
                txt_buffer.push_back(tmp);
                tmp.setString(txt.substr(i, txt.size()));
                tmp.setFillColor(sf::Color::Black);
                txt_buffer.push_back(tmp);
                break;
            }
        }
    } else {
        txt_buffer.push_back(std::move(tmp));
    }

    vis_txt_start = static_cast<int>(txt_buffer.size());
    if(vis_txt_start > 9) {
        vis_txt_end = vis_txt_start - 9;
    }
}

/**
 * Scroll in the given direction.
 * Input_arg comes from MouseWheelScrolled_delta which is -1 or +1
 * direction = 1 => UP
 * direction = -1 => DOWN
 */
void Scroll_text_box::scroll(int direction) {
    if (txt_buffer.size() >= 9) {
        if(direction == UP) {
            if(vis_txt_end > 0) {
                vis_txt_end -= scroll_step;
                if(vis_txt_start - vis_txt_end > 9) {
                vis_txt_start -= scroll_step;
                }
            }
        } else if(direction == DOWN) {
            if(vis_txt_start < txt_buffer.size() && vis_txt_start - vis_txt_end == 9) {
                vis_txt_end += scroll_step;
                vis_txt_start += scroll_step;
            }
        }
    }
}
