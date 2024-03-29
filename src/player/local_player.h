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

#ifndef LOCAL_PLAYER_HEADER
#define LOCAL_PLAYER_HEADER

#include <SFML/Network.hpp>
#include <boost/uuid/uuid.hpp>

#include "base_player.h"
#include "gamelogic/base_card.h"

/**
 * Class representing a local player
 */
class Local_player : public Base_player {
	public:
        Local_player(boost::uuids::uuid id);
        Local_player(boost::uuids::uuid id, std::string player_name);
        Local_player(const Local_player&) = delete;
        ~Local_player();
};
#endif
