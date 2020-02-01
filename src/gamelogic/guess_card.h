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

#ifndef GUESS_CARD_HEADER
#define GUESS_CARD_HEADER

#include "base_card.h"
#include <iostream>

class Guess_card : public Base_card {
public:
    Guess_card(int guess, sf::IntRect tex_pos);

    Guess_card(const Guess_card& rhs) :
        Base_card(rhs.tex_rec,
                  rhs.guess_amount >= 0 ? Guess_card::card_texture : Guess_card::minus_tex) {
        guess_amount = rhs.guess_amount;
        tex_rec = rhs.tex_rec;
    }
    ~Guess_card();

    int guess_amount;

    bool operator==(const Guess_card& rhs) const;

    sf::IntRect tex_rec;

    static sf::Texture card_texture;
    static sf::Texture minus_tex;
};

#endif
