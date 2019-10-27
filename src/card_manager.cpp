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

#include "card_manager.h"
#include <random>

#include <iostream>

Card_Manager::Card_Manager() : deck(repr_deck) {
	std::cout << "size deck: " << this->deck.size() << std::endl;
}

Card_Manager::~Card_Manager() {
}

void Card_Manager::drawCard(Player drawPlayer) {

}

void Card_Manager::shuffleDeck() {
}
