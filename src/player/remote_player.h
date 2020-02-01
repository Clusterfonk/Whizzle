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

#ifndef REMOTE_PLAYER_HEADER
#define REMOTE_PLAYER_HEADER

#include <assert.h>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <SFML/Network.hpp>

#include "base_player.h"

/**
 * Class representing a remote_player
 */
class Remote_player : public Base_player {
	public:
        Remote_player(std::unique_ptr<sf::TcpSocket>* assigned_socket, boost::uuids::uuid id);
        Remote_player(std::unique_ptr<sf::TcpSocket>* assigned_socket, boost::uuids::uuid id,
                std::string player_name);
         ~Remote_player();
        Remote_player(const Remote_player&) = delete;

        sf::TcpSocket* get_socket(); 

        sf::Time ping_timer = sf::Time::Zero;
        bool pending_ping = false;

    private:
        std::unique_ptr<sf::TcpSocket> socket = nullptr;
};
#endif
