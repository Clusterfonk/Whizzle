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

#ifndef CARD_MANAGER_HEADER
#define CARD_MANAGER_HEADER
#include <vector>
#include "card.h"
#include "player.h"


class Card_Manager {
	public:
		Card_Manager();
		~Card_Manager();

		int static const MAX_CARDS_IN_DECK = 60;
		std::vector<Card> deck;
		void drawCard(Player drawPlayer);
		void shuffleDeck();
	private:
};

std::vector<Card> static const repr_deck {
	Card(card_color::blue, 1),
	Card(card_color::blue, 2),
	Card(card_color::blue, 3),
	Card(card_color::blue, 4),
	Card(card_color::blue, 5),
	Card(card_color::blue, 6),
	Card(card_color::blue, 7),
	Card(card_color::blue, 8),
	Card(card_color::blue, 9),
	Card(card_color::blue, 10),
	Card(card_color::blue, 11),
	Card(card_color::blue, 12),
	Card(card_color::blue, 13),
	Card(card_color::red, 1),
	Card(card_color::red, 2),
	Card(card_color::red, 3),
	Card(card_color::red, 4),
	Card(card_color::red, 5),
	Card(card_color::red, 6),
	Card(card_color::red, 7),
	Card(card_color::red, 8),
	Card(card_color::red, 9),
	Card(card_color::red, 10),
	Card(card_color::red, 11),
	Card(card_color::red, 12),
	Card(card_color::red, 13),
	Card(card_color::green, 1),
	Card(card_color::green, 2),
	Card(card_color::green, 3),
	Card(card_color::green, 4),
	Card(card_color::green, 5),
	Card(card_color::green, 6),
	Card(card_color::green, 7),
	Card(card_color::green, 8),
	Card(card_color::green, 9),
	Card(card_color::green, 10),
	Card(card_color::green, 11),
	Card(card_color::green, 12),
	Card(card_color::green, 13),
	Card(card_color::yellow, 1),
	Card(card_color::yellow, 2),
	Card(card_color::yellow, 3),
	Card(card_color::yellow, 4),
	Card(card_color::yellow, 5),
	Card(card_color::yellow, 6),
	Card(card_color::yellow, 7),
	Card(card_color::yellow, 8),
	Card(card_color::yellow, 9),
	Card(card_color::yellow, 10),
	Card(card_color::yellow, 11),
	Card(card_color::yellow, 12),
	Card(card_color::yellow, 13),
	Card(card_color::neutral, CARD_VALUE_NARR),
	Card(card_color::neutral, CARD_VALUE_NARR),
	Card(card_color::neutral, CARD_VALUE_NARR),
	Card(card_color::neutral, CARD_VALUE_NARR),
	Card(card_color::neutral, CARD_VALUE_WHIZZLE),
	Card(card_color::neutral, CARD_VALUE_WHIZZLE),
	Card(card_color::neutral, CARD_VALUE_WHIZZLE),
	Card(card_color::neutral, CARD_VALUE_WHIZZLE)
};
#endif
