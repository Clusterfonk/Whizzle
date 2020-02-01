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

#include "base_player.h"

Base_player::Base_player(boost::uuids::uuid id) {
    unique_id = std::move(id);
}

Base_player::Base_player(boost::uuids::uuid id, std::string player_name) {
    unique_id = std::move(id);
    name = std::move(player_name);
}

Base_player::Base_player() {
}

Base_player::~Base_player(){
}

std::string Base_player::get_name() {
    return name;
}

void Base_player::set_name(std::string player_name) {
    name = player_name;
}

/**
 */
void Base_player::set_unique_id(boost::uuids::uuid id) {
    unique_id = std::move(id);
}

/**
  Returns players unique_id.
 */
boost::uuids::uuid Base_player::get_unique_id() {
      return unique_id;
}
