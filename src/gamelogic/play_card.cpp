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

#include "play_card.h"

sf::Texture Play_card::card_texure = Resource_manager::get_texture("../assets/cards.png");

Play_card::Play_card(card_color color, card_value value, sf::IntRect tex_pos) :
    Base_card(tex_pos, Play_card::card_texure), color(color), value(value) {
}

Play_card::~Play_card() {
}

bool Play_card::operator==(const Play_card &rhs) const {
    return (rhs.color == color &&
            rhs.value == value);
}

bool Play_card::operator!=(const Play_card& rhs) const {
    return !(this == &rhs);
}

void Play_card::max(Play_card* lhs, Play_card*& max, card_color*& dealing_color, card_color*& trump_color) {
    if(!max) {
        max = lhs;
    } else if(lhs) {

        if(lhs->color == card_color::neutral) {
                if(is_whiz(lhs->value) && !is_whiz(max->value)) {
                    max = lhs;
                }
        } else {

            if(!is_whiz(max->value)) {

                if(trump_color && dealing_color) {

                    if(lhs->color == *trump_color) {
                        if(max->color ==*trump_color) {
                            max = lhs > max ? lhs : max;
                        } else {
                            max = lhs;
                        }
                    } else if(lhs->color == *dealing_color) {
                        if(max->color == *dealing_color) {
                            max = lhs > max ? lhs : max;
                        } else if(max->color != *trump_color &&
                                  max->color != *dealing_color) {
                            max = lhs;
                        }
                    } else {
                        if(max->color != *trump_color &&
                                max->color != *dealing_color) {
                            max = lhs > max ? lhs : max;
                        }
                    }

                } else if(!trump_color && dealing_color) {
                    if(lhs->color == *dealing_color) {
                        if(max->color == *dealing_color) {
                            max = lhs > max ? lhs : max;
                        } else {
                            max = lhs;
                        }
                    } else {
                        max = lhs > max ? lhs : max;
                    }
                }
            }

        }
    }

}

void Play_card::reset() {
    card_sprite.setScale(1.485f, 1.485f);
    moveable = true;
    is_spawned = false;
}
