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

#include "base_state.h"

Base_state::Base_state(State_manager& s_m, sf::RenderWindow& window, bool replace) :
    state_manager(s_m), window(window), marked_replace(replace) {
       marked_clear = replace;
}

std::unique_ptr<Base_state> Base_state::get_next() {
	return std::move(next);
}

bool Base_state::is_replacing() {
	return marked_replace;
}

bool Base_state::is_clearing() {
	return marked_clear;
}
