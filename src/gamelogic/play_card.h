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

#ifndef PLAY_CARD_HEADER
#define PLAY_CARD_HEADER

#include <assert.h>

#include "base_card.h"

/**
 * Enum class representing the color of a card
 */
enum card_color {
    neutral,
    blue,
    red,
    green,
    yellow
};

/**
 * Enum class representing the value of a card
 */
enum card_value {
    narr_1, narr_2, narr_3, narr_4,
    one, two, three, four, five, six, seven, eight, nine, ten, eleven, twelve, thirteen,
    whizzle_1, whizzle_2, whizzle_3, whizzle_4
};

inline bool is_whiz(card_value c) {
    return (c == card_value::whizzle_1 || c == card_value::whizzle_2 || c == card_value::whizzle_3 || c == card_value::whizzle_4);
}

inline bool is_narr(card_value c) {
    return (c == card_value::narr_1 || c == card_value::narr_2 || c == card_value::narr_3 || c == card_value::narr_4);
}


/**
 * Playable cards
 */
class Play_card : public Base_card {
    public:
        Play_card(card_color color, card_value value, sf::IntRect tex_pos);
        Play_card();
        ~Play_card();

        void reset();

        card_color color;
        card_value value;

        bool operator==(const Play_card& rhs) const;
        bool operator!=(const Play_card& rhs) const;

        static void max(Play_card* lhs, Play_card*& max, card_color*& dealing_color, card_color*& trump_color);
        static sf::Texture card_texure;
};
#endif
