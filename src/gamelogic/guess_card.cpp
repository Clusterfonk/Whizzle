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

#include "guess_card.h"

sf::Texture Guess_card::card_texture = Resource_manager::get_texture("../assets/guess_cards.png");
sf::Texture Guess_card::minus_tex = Resource_manager::get_texture("../assets/minus_guess_cards.png");

Guess_card::Guess_card(int guess, sf::IntRect tex_pos)
    : Base_card(tex_pos, guess >= 0 ? Guess_card::card_texture : Guess_card::minus_tex) {
    guess_amount = guess;
    tex_rec = tex_pos;
}

Guess_card::~Guess_card() {
}

bool Guess_card::operator==(const Guess_card &rhs) const {
    return rhs.guess_amount == guess_amount;
}
