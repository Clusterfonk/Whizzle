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

#ifndef CONVO_HEADER
#define CONVO_HEADER

#include <bitset>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <type_traits>

#include "gamelogic/play_card.h"
#include "manager/game_manager.h"
#include "network/base_network_component.h"

/**
 * Converts strongly typed Enum to corresponding underlying_type
 */
template <typename E>
    constexpr auto to_underlying(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }

sf::Packet& operator <<(sf::Packet& packet, const boost::uuids::uuid& id);
sf::Packet& operator >>(sf::Packet& packet, boost::uuids::uuid& id);

sf::Packet& operator <<(sf::Packet& packet, const size_t& size);
sf::Packet& operator >>(sf::Packet& packet, size_t& size);

sf::Packet& operator <<(sf::Packet& packet, const SIGNAL& signal);
sf::Packet& operator >>(sf::Packet& packet, SIGNAL& signal);

sf::Packet& operator <<(sf::Packet& packet, const Game_manager::STATE& state);
sf::Packet& operator >>(sf::Packet& packet, Game_manager::STATE& state);

sf::Packet& operator <<(sf::Packet& packet, const card_color& color);
sf::Packet& operator >>(sf::Packet& packet, card_color& color);
sf::Packet& operator <<(sf::Packet& packet, const card_value& value);
sf::Packet& operator >>(sf::Packet& packet, card_value& value);

std::string base64_encode(sf::IpAddress& ip);
sf::IpAddress base64_decode(std::string& enc_addr);

#endif
