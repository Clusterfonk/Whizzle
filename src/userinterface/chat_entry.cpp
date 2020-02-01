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

#include "chat_entry.h"

Chat_entry::Chat_entry(sf::Vector2f pos, unsigned int size, unsigned int max_size) :
    Text_entity (pos, size, false, max_size) {
    text.setFillColor(sf::Color::Black);
}

std::string Chat_entry::get_string() {
    rm_indicator();
    std::string ret = text.getString();
    if(ret[0] == indicator) {
        ret = ret.substr(1, ret.size());
    }
    return ret;
}

void Chat_entry::draw(sf::RenderTarget &target) {
    if(active) {
        target.draw(text);
    }
}

void Chat_entry::clear() {
    rm_indicator();
    text.setString(std::string("") + indicator);
}
