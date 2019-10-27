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

Network_Manager::Network_Manager() {
	this->ip_addr = sf::IpAddress::getPublicAddress();
}

Network_Manager::~Network_Manager() {
}

sf::Socket::Status Network_Manager::listen() {
	sf::Socket::Status con_status = tcp_listener.listen(this->port);
	if (con_status != sf::Socket::Done) {
		printf("listen error");
		return con_status;
	}
	sf::TcpSocket socket;
	con_status = tcp_listener.accept(socket);
	if (con_status != sf::Socket::Done) {
		printf("accept error");
		return con_status;
	} 
	this->con_list.push_back(&socket);
	return con_status;
}

sf::Socket::Status Network_Manager::connect(const sf::IpAddress &ip) {
	sf::TcpSocket socket;
	sf::Socket::Status con_status = socket.connect(ip, this->port);
	if(con_status != sf::Socket::Done) {
		printf("Couldn't establish connection with %s\n", ip.toString().c_str());
		return con_status;
	}
	this->con_list.push_back(&socket);
	return con_status;
}

// TODO
void Network_Manager::establish_connection() {
	//this->listen();

	//this->connect(); //needs ip of which failed to connect
}

void Network_Manager::broadcast_msg(){
}
