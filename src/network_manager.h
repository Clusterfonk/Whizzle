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

#ifndef NETWORK_MANAGER_HEADER
#define NETWORK_MANAGER_HEADER

#include <list>
#include <SFML/Network.hpp>
#include <stdio.h>

class Network_Manager {
	public:
		Network_Manager();
		~Network_Manager();

		sf::TcpSocket tcp_socket;
		sf::TcpListener tcp_listener;
		
		sf::IpAddress ip_addr;
		unsigned short const static port = 50001;

		void update();

		std::list<sf::TcpSocket*> con_list;

		sf::Socket::Status listen();

		sf::Socket::Status connect(const sf::IpAddress &ip);

		void establish_connection();

		void broadcast_msg();
};
#endif
