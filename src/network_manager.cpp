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

#include "network_manager.h"
#include <iostream>

Network_Manager::Network_Manager() {
    connection_count = 0;
	//ip_addr = sf::IpAddress::getPublicAddress();
    ip_addr = "127.0.0.1"; // DEBUG!
    max_connections = 6;

    l_thread = new std::thread(&Network_Manager::receive, this);
    l_thread->detach();
    printf("thread launching\n");
}

Network_Manager::~Network_Manager() {
    isListening = false;
}

void Network_Manager::receive() {
    selector.add(tcp_listener);
    isListening = true;
    if (tcp_listener.listen(this->port) != sf::Socket::Done) {
    std::cout << "Started listening to incoming connection requests" << std::endl;
    }

    while(isListening) {
        if (selector.wait()) {
            if (selector.isReady(tcp_listener)) {
                std::unique_ptr<sf::TcpSocket> socket = std::make_unique<sf::TcpSocket>();
                if (tcp_listener.accept(*socket) == sf::Socket::Done) {
                    if (connection_count < max_connections) { // Server not full
                        printf("accepted connection\n");
                        l_mutex.lock(); // !Lock 
                        player_list->emplace_back(Player(&socket, connection_count));
                        selector.add(*player_list->back().GetSocket());
                        connection_count++;
                        this->l_mutex.unlock(); // !Unlock
                    } else { // Server full

                    }
                } 
            } else {
                for(auto& player: *player_list) {
                    if(selector.isReady(*player.GetSocket())) {
                        sf::Packet packet;
                        if (player.GetSocket()->receive(packet) == sf::Socket::Done) {
                            receivedPackets.push(packet);
                        }
                    }
                }
            }
        }
    }
}


/**
sf::Socket::Status Network_Manager::connect(const sf::IpAddress &ip) {
    std::unique_ptr<sf::TcpSocket> socket = std::make_unique<sf::TcpSocket>();
	sf::Socket::Status con_status = socket->connect(ip, this->port);
	if(con_status != sf::Socket::Done) {
		printf("Couldn't establish connection with %s\n", ip.toString().c_str());
		return con_status;
	}
    this->player_list->emplace_back(Player(&socket, connection_count));
	return con_status;
}*/

void Network_Manager::broadcast_msg(){
}
