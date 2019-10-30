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
#include <SFML/System.hpp>
#include <queue>
#include "player.h"
#include <thread>
#include <memory>

#define MAX_MSG_BUFFER 2048

class Network_Manager {
	public:
		Network_Manager();
		~Network_Manager();

		sf::TcpSocket tcp_socket;
		
		sf::IpAddress ip_addr;
		unsigned short const static port = 8000;

        int connection_count;

		void update();

		std::list<Player>* player_list; // SHARED RESOURCE!

		sf::Socket::Status connect(const sf::IpAddress &ip);

		void broadcast_msg();

        bool isListening = false;

    private:
		void receive();
        unsigned short max_connections;

		sf::TcpListener tcp_listener;
        sf::SocketSelector selector;
        std::thread* l_thread = nullptr;
        sf::Mutex l_mutex;

        std::queue<sf::Packet> receivedPackets;
        char msg_buffer[MAX_MSG_BUFFER];
};
#endif
