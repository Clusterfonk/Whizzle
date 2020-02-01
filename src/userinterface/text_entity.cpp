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

#include "text_entity.h"
#include <iostream>

Text_entity::Text_entity() : MAX_STRING_SIZE(25) {
    text.setFont(Resource_manager::get_font("../assets/apercu_mono.ttf"));
}

Text_entity::Text_entity(sf::Vector2f pos, unsigned int size, 
        bool should_resize, unsigned int max_size) : origin(pos),
        resizing(should_resize), MAX_STRING_SIZE(max_size) {
    text.setFont(Resource_manager::get_font("../assets/apercu_mono.ttf"));
    text.setCharacterSize(size);
    text.setString(indicator);
    set_position(pos);
    active = true;
}

Text_entity::~Text_entity() {
}

void Text_entity::update(const sf::Time& delta_time) {
    if(active) {
        indicator_timer += delta_time.asSeconds();
        if(indicator_timer >= blink_timer && !indication && 
                !(text.getString().getData()[text.getString().getSize() - 1] ==
                static_cast<uint32_t>(indicator))) {
            indication = true;
            text.setString(text.getString() + indicator);
        } else if(indicator_timer >= 2 * blink_timer) {
            rm_indicator();
            indication = false;
            blink_timer = 0.7f;
            indicator_timer = 0.0f;
        }
    }
}

void Text_entity::draw(sf::RenderTarget& target) {
    target.draw(text);
}

bool Text_entity::add_char(char in_char) {
    sf::String tmp = text.getString();
    if ((tmp.getData()[tmp.getSize() - 1] == 
                static_cast<uint32_t>(indicator)) && indication) {
        tmp.erase(tmp.getSize() - 1);
    }
    if(tmp.getSize() >= MAX_STRING_SIZE) {
        indication = true;
        indicator_timer = 0.0f;
        blink_timer = 0.35f;
        return false;
    }
    text.setString(tmp + in_char);
    if (resizing) resize();
    text.setString(text.getString() + indicator);

    indication = true;
    indicator_timer = 0.0f;
    blink_timer = 0.35f;
    return true;
}

void Text_entity::rm_char() {
    sf::String tmp = text.getString();
    if ((tmp.getData()[tmp.getSize() - 1] == 
                static_cast<uint32_t>(indicator)) && indication) {
        tmp.erase(tmp.getSize() - 1);
    }
    if(tmp.getSize() > 0) {
        tmp.erase(tmp.getSize() - 1);
    }
    text.setString(tmp + indicator);
    if(resizing) resize();

    indication = true;
    indicator_timer = 0.0f;
    blink_timer = 0.35f;
}

void Text_entity::clear() {
    text.setString(std::string("") + indicator);
}

void Text_entity::set_position(sf::Vector2f pos) {
    origin = pos;
    text.setPosition({pos.x - text.getGlobalBounds().width / 2, 
            pos.y});
}

/**
 * Horizontally resize the Text Entity
 */
void Text_entity::resize() {
    text.setPosition({origin.x - text.getGlobalBounds().width / 2, origin.y});
}

void Text_entity::set_color(int r, int g, int b, int a) {
    alpha_channel = a;
    text.setFillColor(sf::Color(r, g, b, a));
}

inline bool Text_entity::is_active() {
    return active;
}

void Text_entity::set_active(bool set_val) {
    active = set_val;
}

void Text_entity::set_max_size(unsigned int new_size) {
    MAX_STRING_SIZE = new_size;
}

std::string Text_entity::get_text() {
    sf::String tmp = text.getString();
    if (tmp.getData()[tmp.getSize() - 1]
                == static_cast<uint32_t>(indicator) && indication) {
            tmp.erase(tmp.getSize() - 1);
    }
    return tmp;
}

/**
 * Removes the indicator from the text_entity ignoring inputs of indicators
 */
void Text_entity::rm_indicator() {
    // prevents indicator input removal
    if (text.getString().getData()[text.getString().getSize() - 1]
            == static_cast<uint32_t>(indicator) && indication) {
        sf::String tmp = text.getString();
        tmp.erase(tmp.getSize() - 1);
        text.setString(tmp);
    }
}
