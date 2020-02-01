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

#include "room_id_cards.h"
#include <iostream>

Room_id_cards::Room_id_cards(sf::Vector2f origin, std::string room_id) {
    cards_tex = Resource_manager::get_texture("../assets/creation_bar.png");
    cards_sprite.setTexture(cards_tex);
    cards_sprite.setTextureRect(sf::IntRect(385, 0, 612, 177));
    cards_sprite.setPosition(origin.x - 306, origin.y - 88.5);
    Label::Align algn = Label::Align::BOT;
    card_labels.push_back(Label(room_id[0], {origin.x - 300, origin.y - 85}, 80, algn));
    card_labels.push_back(Label(room_id[1], {origin.x - 198, origin.y - 65}, 80, algn));
    card_labels.push_back(Label(room_id[2], {origin.x - 98, origin.y - 85}, 80, algn));
    card_labels.push_back(Label(room_id[3], {origin.x + 5, origin.y - 65}, 80, algn));
    card_labels.push_back(Label(room_id[4], {origin.x + 110, origin.y - 85}, 80, algn));
    card_labels.push_back(Label(room_id[5], {origin.x + 215, origin.y - 65}, 80, algn));
}

Room_id_cards::~Room_id_cards() {
}

void Room_id_cards::draw(sf::RenderTarget& target) {
    target.draw(cards_sprite);
    for(auto& label : card_labels) {
        label.draw(target);
    }
}

void Room_id_cards::update(const sf::Time& delta_time) {
}
