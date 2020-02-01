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

#include "placed_card.h"

Placed_card::Placed_card(sf::Vector2f origin) {
    card_pos = origin;
}

void Placed_card::play(Base_card* played_card) {
    if(!played_card) return;
    card = played_card;
    if(!card->is_spawned) {
        card->spawn({1280.0f / 2.0f, 720.0f / 2.0f}, {1.0f, 1.0f});
    } else {
        card->placed({1280.0f / 2.0f, 720.0f / 2.0f});
    }
    card->moveable = false;
}

void Placed_card::discard() {
    if(card) {
        card->moveable = true;
    }
    card = nullptr;
}

void Placed_card::draw(sf::RenderTarget& target) {
    if(card) {
        card->draw(target);
    }
    if(Game_manager::get_instance()->faceup_card) {
        if(Game_manager::get_instance()->faceup_card->is_spawned) {
            Game_manager::get_instance()->faceup_card->draw(target);
        } else {
            Game_manager::get_instance()->faceup_card->spawn({1280.0f / 2.0f, 175.0f}, {1.0f, 1.0f});
        }
    }
}

void Placed_card::update(sf::Window& window) {
    if(card) {
        card->update(window);
    }
}

Play_card* Placed_card::get_card() {
    return dynamic_cast<Play_card*>(card);
}
