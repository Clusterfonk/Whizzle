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

#include "base_network_component.h"

Base_network_component::Base_network_component() {
}

bool Base_network_component::is_connected() {
    return connection;
}

bool Base_network_component::has_issue() {
    return connection_issue;
}

size_t Base_network_component::get_connection_count() {
    return player_list.size();
}

bool Base_network_component::next_players_turn() {
    if(player_on_turn != player_list.end()) {
        player_on_turn++;
        Player_list::Iterator<Base_player> tmp = player_on_turn;
        tmp++;
        if(player_on_turn != player_list.end() && tmp == player_list.end()) {
            return true;
        }
    }
    return false;
}

void Base_network_component::reorder(boost::uuids::uuid winner_id) {
    enemy_mutex.lock(); // !LOCK
    for(Player_list::Iterator<Base_player> itr = player_list.begin();
        itr != player_list.end() && itr->get_unique_id() != winner_id;
        itr++) {
        player_list.swap_to_end(itr->get_unique_id());
    }
    enemy_mutex.unlock(); // !UNLOCK
}

void Base_network_component::set_chat(Chat* ref_chat) {
    if(ref_chat) {
        chat = ref_chat;
    } else {
        // TODO: Logging
    }
}
