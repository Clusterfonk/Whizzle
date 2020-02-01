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

#include "remote_player.h"
#include <iostream>

Remote_player::Remote_player(std::unique_ptr<sf::TcpSocket>* assigned_socket,
        boost::uuids::uuid id) : Base_player(id) {
    if (assigned_socket) { // Socket is managed by an unique_ptr
        socket = std::move(*assigned_socket);
    }
}

Remote_player::Remote_player(std::unique_ptr<sf::TcpSocket>* assigned_socket,
        boost::uuids::uuid id, std::string player_name) : Base_player(id, player_name) {
    if (assigned_socket) { // Socket is managed by an unique_ptr
        socket = std::move(*assigned_socket);
    }
}

Remote_player::~Remote_player(){
    if (socket) {
        socket.reset();
    }
}

sf::TcpSocket* Remote_player::get_socket() { 
    return socket ? socket.get() : nullptr;
}
