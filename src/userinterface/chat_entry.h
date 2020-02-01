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

#ifndef CHAT_ENTRY_HEADER
#define CHAT_ENTRY_HEADER

#include <SFML/Graphics.hpp>

#include "manager/resource_manager.h"
#include "text_entity.h"

class Chat_entry : public Text_entity {
public:
    Chat_entry();
    Chat_entry(sf::Vector2f pos, unsigned int size, unsigned int max_size);

    std::string get_string();
    void draw(sf::RenderTarget &target);

    void clear();
};

#endif
