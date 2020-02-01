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

#include "label.h"

Label::Label() {
}

Label::Label(const sf::String& text, sf::Vector2f pos, unsigned int size, 
        Align align) : alignment(align), origin(pos) {
    switch (align) {
        case Align::TOP:
            {
                txt.setFont(Resource_manager::get_font("../assets/trajan_pro.ttf"));
                txt.setString(text);
                txt.setCharacterSize(size);
                txt.setFillColor(sf::Color(255, 255, 255, 255));
                txt.setPosition(pos.x - (txt.getGlobalBounds().width / 2) - 8,
                        pos.y - (txt.getGlobalBounds().height / 2) - 8);
                break;
            }
        case Align::BOT:
            {
                txt.setFont(Resource_manager::get_font("../assets/apercu_mono.ttf"));
                txt.setString(text);
                txt.setCharacterSize(size);
                txt.setFillColor(sf::Color(255, 255, 255, 255));
                txt.setPosition(pos.x + (txt.getGlobalBounds().width / 2),
                        pos.y + (txt.getGlobalBounds().height / 2));
                break;
            }
    }
}

void Label::set_string(const std::string& text) {
    txt.setString(text);
}

void Label::set_alignment(Align a) {
    alignment = a;
}

void Label::resize() {
    set_position(origin);
}

void Label::set_position(sf::Vector2f pos) {
    switch(alignment) {
        case Align::TOP:
            {
                txt.setPosition(pos.x - (txt.getGlobalBounds().width / 2) - 8,
                        pos.y - (txt.getGlobalBounds().height / 2) - 8);
                break;
            }
        case Align::BOT:
            {
                txt.setPosition(pos.x + (txt.getGlobalBounds().width / 2),
                        pos.y + (txt.getGlobalBounds().height / 2));
                break;
            }
    }
}

void Label::set_size(unsigned int size) {
    txt.setCharacterSize(size);
}

void Label::draw(sf::RenderTarget& target) {
    target.draw(txt);
}

void Label::set_color(int r, int g, int b, int a) {
    txt.setFillColor(sf::Color(static_cast<sf::Uint8>(r),
                               static_cast<sf::Uint8>(g),
                               static_cast<sf::Uint8>(b),
                               static_cast<sf::Uint8>(a)));
}
