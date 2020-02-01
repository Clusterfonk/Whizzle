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

#ifndef ROOM_ID_CARDS_HEADER
#define ROOM_ID_CARDS_HEADER

#include <SFML/Graphics.hpp>
#include <vector>

#include "label.h"
#include "manager/input_manager.h"
#include "manager/resource_manager.h"

class Room_id_cards {
    public:
        Room_id_cards(sf::Vector2f origin, std::string room_id);
        ~Room_id_cards();

        void draw(sf::RenderTarget& target);
        void update(const sf::Time& delta_time);

    private:
        sf::Texture cards_tex;
        sf::Sprite cards_sprite;
        std::vector<Label> card_labels;
};
#endif
