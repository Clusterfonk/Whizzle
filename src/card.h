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

#ifndef CARD_HEADER
#define CARD_HEADER

#include <assert.h>
#include <SFML/Graphics.hpp>

enum class card_color {neutral, blue, red, green, yellow};
#define CARD_VALUE_NARR 0
#define CARD_VALUE_WHIZZLE 14

class Card {
	public:
		Card(card_color color, int value); 
		~Card();
		
		card_color color;
		int value; 
		sf::Sprite cover;
		sf::Sprite static backdrop;
};
#endif
