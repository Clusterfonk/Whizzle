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

#include "convo.h"

namespace {
    // Alternative Base64 alphabet lookup table
    static const std::string base64_chars = 
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz"
                    "0123456789+-"; 
}

/**
 * Converts unique_id into a packet conform type and append it to the packet
 */
sf::Packet& operator <<(sf::Packet& packet, const boost::uuids::uuid& id) {
    return packet << boost::lexical_cast<std::string>(id);
}

/**
 * Receive unique_id from a packet
 */
sf::Packet& operator >>(sf::Packet& packet, boost::uuids::uuid& id) {
    std::string tmp;
    packet >> tmp;
    id = boost::lexical_cast<boost::uuids::uuid>(tmp);
    return packet;
}

/**
 * Converts size into a packet conform type and append it to the packet
 */
sf::Packet& operator <<(sf::Packet& packet, const size_t& size) {
    unsigned int tmp = static_cast<unsigned int>(size);
    return packet << tmp;
}

/**
 * Receive size_t from a packet
 */
sf::Packet& operator >>(sf::Packet& packet, size_t& size) {
    unsigned int tmp;
    packet >> tmp;
    size = static_cast<size_t>(tmp);
    return packet;
}

/**
 * Converts Network SIGNAL into a packet conform type and append it to the packet
 */
sf::Packet& operator <<(sf::Packet& packet, const SIGNAL& signal) {
    int tmp = to_underlying(signal);
    return packet << tmp;
}

/**
 * Receive Network SIGNAL from a packet
 */
sf::Packet& operator >>(sf::Packet& packet, SIGNAL& signal) {
    int tmp;
    packet >> tmp;
    signal = static_cast<SIGNAL>(tmp);
    return packet;
}

/**
 * Converts Game_State into a packet conform type and append it to the packet
 */
sf::Packet& operator <<(sf::Packet& packet, const Game_manager::STATE& state) {
    int tmp = to_underlying(state);
    return packet << tmp;
}

/**
 * Receive Game_State from a packet
 */
sf::Packet& operator >>(sf::Packet& packet, Game_manager::STATE& state) {
    int tmp;
    packet >> tmp;
    state = static_cast<Game_manager::STATE>(tmp);
    return packet;
}

/**
 * Converts card_color into a packet conform type and append it to the packet
 */
sf::Packet& operator <<(sf::Packet& packet, const card_color& color) {
    unsigned int tmp = to_underlying(color);
    return packet << tmp;
}

/**
 * Receive card_color from a packet
 */
sf::Packet& operator >>(sf::Packet& packet, card_color& color) {
    unsigned int tmp;
    packet >> tmp;
    color = static_cast<card_color>(tmp);
    return packet;
}

/**
 * Converts card_value into a packet conform type and append it to the packet
 */
sf::Packet& operator <<(sf::Packet& packet, const card_value& value) {
    unsigned int tmp = to_underlying(value);
    return packet << tmp;
}

/**
 * Receive card_value from a packet
 */
sf::Packet& operator >>(sf::Packet& packet, card_value& value) {
    unsigned int tmp;
    packet >> tmp;
    value = static_cast<card_value>(tmp);
    return packet;
}

// TODO: REDO USING BITMASKING!

/**
 * Enocdes an IpAddress into a readable 6 letter word
 * using Base64 encoding with alternative alphabet
 */
std::string base64_encode(sf::IpAddress& ip) {
    std::string ret;
    std::bitset<32> bits(ip.toInteger());
    std::bitset<6> chunk;
    for(int i = 0; i <= 5; i++) { 
        chunk[5] = 31 - 6*i >= 0 ? bits[31 - 6*i] : 0;
        chunk[4] = 30 - 6*i >= 0 ? bits[30 - 6*i] : 0;
        chunk[3] = 29 - 6*i >= 0 ? bits[29 - 6*i] : 0;
        chunk[2] = 28 - 6*i >= 0 ? bits[28 - 6*i] : 0;
        chunk[1] = 27 - 6*i >= 0 ? bits[27 - 6*i] : 0;
        chunk[0] = 26 - 6*i >= 0 ? bits[26 - 6*i] : 0;
        ret += base64_chars[chunk.to_ulong()];
    }
    return ret;
}

// TODO: REDO USING BITMASKING!

/**
 * Decodes a 6 letter string back into an IpAddress
 * using Base64 decoding with alternative alphabet
 */
sf::IpAddress base64_decode(std::string& enc_addr) {
    std::bitset<32> rev;
    std::bitset<6> rev_chunk;
    int it = 0;
    for(auto& cr : enc_addr) {
        rev_chunk = std::bitset<6>(base64_chars.find(cr));
        if(31-6*it >= 0) rev[31 - 6*it] = rev_chunk[5];
        if(30-6*it >= 0) rev[30 - 6*it] = rev_chunk[4];
        if(29-6*it >= 0) rev[29 - 6*it] = rev_chunk[3];
        if(28-6*it >= 0) rev[28 - 6*it] = rev_chunk[2];
        if(27-6*it >= 0) rev[27 - 6*it] = rev_chunk[1];
        if(26-6*it >= 0) rev[26 - 6*it] = rev_chunk[0];
        it++;
    }
    return sf::IpAddress(rev.to_ulong()).toString();
}
