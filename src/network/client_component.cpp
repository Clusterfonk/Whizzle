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

#include "client_component.h" 
#include <iostream>

Client_component::Client_component() : Base_network_component() {
}

Client_component::~Client_component() {
        reset_component();
}

/**
 * Resets the network component
 */
void Client_component::reset_component(bool clean) {
    if(clean) {
        disconnect();
    } else {
        force_disconnect();
    }
    player_list.clear();
}

/**
 * Connect to a host by given encoded ip address
 */
bool Client_component::connect(std::string ip_encoded) {
    sf::IpAddress ip = base64_decode(ip_encoded);
    // Prevent connection to oneself
//    if (ip == sf::IpAddress("127.0.0.1")) return false; commented for DEBUGGING
    std::unique_ptr<sf::TcpSocket> socket = std::make_unique<sf::TcpSocket>();
    if(socket->connect(ip, port, sf::seconds(3.0f)) != sf::Socket::Done) {
        std::cout << "Connection Timedout!" << std::endl;
        return false;
    }
    socket->setBlocking(false);
    player_list.emplace_back(new Remote_player(&socket, boost::uuids::nil_uuid(), "Player 1"));
    connection = true;
    std::cout << "connected to host" << std::endl;
    return true;
}

/**
 * Send disconnect information and disconnect from host
 */
void Client_component::disconnect() {
    if(connection) {
        sf::Packet snd_packet;
        snd_packet << SIGNAL::PLAYER_DISCONNECTED << player_list.get_self().get_unique_id();
        if(player_list.get_host().get_socket()->send(snd_packet) != sf::Socket::Done) {
            connection_issue = true;
            std::cout << "Couldn't send disconnnect packet" << std::endl;
        }
        player_list.get_host().get_socket()->disconnect();
        std::cout << "Disconnected from Host" << std::endl;
        connection = false;
    }
}

void Client_component::force_disconnect() {
    if(connection) {
        player_list.get_host().get_socket()->disconnect();
        std::cout << "Disconnected forcefully from Host" << std::endl;
        connection = false;
    }
}

/**
 * Processes incoming packets from the host
 */
void Client_component::update(const sf::Time& delta_time) {
    if(connection) {
        sf::Packet rec_packet;
        SIGNAL signal;
        player_list.get_host().get_unique_id();
        if(player_list.get_host().get_socket()->receive(rec_packet) == sf::Socket::Done) {
            rec_packet >> signal;
            switch(signal) {
                case SIGNAL::PLAYER_INFO:
                    {
                        boost::uuids::uuid id;
                        std::string name;
                        rec_packet >> id;
                        rec_packet >> name;
                        player_list.get_host().set_unique_id(id);
                        player_list.get_host().set_name(name);
                        rec_packet >> id;
                        rec_packet >> name;
                        player_list.emplace_back(new Local_player(id, name));
                        is_pinging = true;
                        break;
                    }
                case SIGNAL::HOST_DISCONNECTED:
                    {
                        reset_component();
                        break;
                    }
                case SIGNAL::SESSION_FULL:
                    {
                        reset_component(false);
                        break;
                    }
                case SIGNAL::PING_REQ:
                    {
                        send_ping();
                        ping_time = sf::Time::Zero;
                        break;
                    }
                case SIGNAL::SESSION_PLAY:
                    {
                        is_session_started = true;
                        break;
                    }
                case SIGNAL::PLAYER_LIST:
                    {
                        size_t c_count;
                        rec_packet >> c_count;
                        std::string tmp_name;
                        boost::uuids::uuid tmp_id;
                        for(unsigned int i = 0; i < c_count; i++) {
                            rec_packet >> tmp_id;
                            rec_packet >> tmp_name;
                            if(player_list.contains(tmp_id)) {
                                player_list.swap_to_end(tmp_id);
                            } else {
                                player_list.emplace_back(new Remote_player(nullptr, tmp_id, tmp_name));
                            }
                        }
                        player_on_turn = player_list.begin();

                        for(Player_list::Iterator<Base_player> itr = player_list.begin();
                            itr != player_list.end();
                            itr++) {
                            Game_manager::get_instance()->initial_play_order.push_back(itr->get_unique_id());
                        }
                        Game_manager::get_instance()->i_p_o_itr = Game_manager::get_instance()->initial_play_order.begin();
                        Game_manager::get_instance()->is_player_setup = true;
                        break;
                    }
                case SIGNAL::PLAYER_READY:
                    {
                        Game_manager::get_instance()->start_game();
                        break;
                    }
                case SIGNAL::NEXT_STATE:
                    {
                        int state;
                        rec_packet >> state;
                        Game_manager::get_instance()->state = static_cast<Game_manager::STATE>(state);
                        if(Game_manager::get_instance()->state == Game_manager::STATE::PLAY_CARD ||
                                Game_manager::get_instance()->state == Game_manager::STATE::PLAY_GUESS) {
                            Game_manager::get_instance()->is_play_order_setup = false;
                        }
                        is_ready = false;
                        break;
                    }
                case SIGNAL::PLAY_CARD:
                    {
                        card_color c;
                        card_value v;
                        rec_packet >> c >> v;
                        hand_buffer.push_back(get_play_card(c, v));
                        pending_card_req = false;
                        break;
                    }
                case SIGNAL::FACEUP_CARD:
                    {
                        bool has_faceup;
                        rec_packet >> has_faceup;
                        if(has_faceup) {
                            card_color c;
                            card_value v;
                            rec_packet >> c >> v;
                            Game_manager::get_instance()->faceup_card = get_play_card(c, v);
                            if(Game_manager::get_instance()->faceup_card) {
                                if(Game_manager::get_instance()->faceup_card->color != card_color::neutral) {
                                    Game_manager::get_instance()->trump_color = &Game_manager::get_instance()->faceup_card->color;
                                }
                            }
                        } else {
                            Game_manager::get_instance()->faceup_card = nullptr;
                            Game_manager::get_instance()->trump_color = nullptr;
                        }

                        Game_manager::get_instance()->is_faceup_drawn = true;
                        is_card_received = true;
                        break;
                    }
                case SIGNAL::CONNECTION_ISSUE:
                    {
                        connection_issue = true;
                        reset_component();
                        break;
                    }
                case SIGNAL::WAS_PLAYED_CARD:
                    {
                        card_color c;
                        card_value v;
                        rec_packet >> c >> v;
                        Play_card* played_card = get_play_card(c,v);
                        player_on_turn->played_card = played_card;
                        if(!Game_manager::get_instance()->dealing_color) {
                            if(played_card && played_card->color != card_color::neutral) {
                                Game_manager::get_instance()->dealing_color = &played_card->color;
                            }
                        }
                        Game_manager::get_instance()->is_last = next_players_turn();
                        break;
                    }
                case SIGNAL::WAS_PLAYED_GUESS:
                    {
                        int guess;
                        rec_packet >> guess;
                        player_on_turn->played_guess = get_guess_card(guess);
                        Game_manager::get_instance()->total_guesses += static_cast<unsigned int>(guess);
                        Game_manager::get_instance()->is_last = next_players_turn();
                        break;
                    }
                case SIGNAL::DECK_SIZE:
                    {
                        int size;
                        rec_packet >> size;
                        Card_manager::get_instance()->deck_size = size;
                        break;
                    }
                case SIGNAL::CHAT_MSG:
                    {
                        std::string msg;
                        rec_packet >> msg;

                        boost::uuids::uuid sender_id;
                        rec_packet >> sender_id;

                        if(chat) {
                            for(Player_list::Iterator<Base_player> itr = player_list.begin();
                                itr != player_list.end(); itr++) {
                                if(itr->get_unique_id() == sender_id) {
                                    chat->add_msg("[" + itr->get_name() + "]: " + msg);
                                }
                            }
                        }
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }
        /**
          * Advance ping timer and disconnect if timedout
          */
        if(is_pinging) {
            ping_time += delta_time;
            // Timeout
            if(ping_time >= ping_time_limit) {
                std::cout << "ping timeout" << std::endl;
                connection_issue = true;
                reset_component();
            }
        }
    }
}

/**
 * Sends a ping signal to the host
 */
void Client_component::send_ping() {
    sf::Packet packet;
    packet << SIGNAL::PING << player_list.get_self().get_unique_id();
    if(player_list.get_host().get_socket()->send(packet) != sf::Socket::Done) {
        std::cout << "Couldn't respond to ping" << std::endl;
    }
}

/**
 * Sends a request to receive the player list from host
 */
void Client_component::send_player_list_req() {
    sf::Packet packet;
    packet << SIGNAL::PLAYER_LIST_REQ << player_list.get_self().get_unique_id();
    if(player_list.get_host().get_socket()->send(packet) != sf::Socket::Done) {
        std::cout << "Couldn't send list req" << std::endl;
    }
}

void Client_component::send_ready_play() {
    if(!is_ready) {
        sf::Packet packet;
        packet << SIGNAL::PLAYER_READY << player_list.get_self().get_unique_id();
        if(player_list.get_host().get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Couldn't send Game Ready" << std::endl;
        }
       is_ready = true;
    }
}

void Client_component::send_card_request() {
    if(!pending_card_req) {
        sf::Packet packet;
        packet << SIGNAL::PLAY_CARD_REQ << player_list.get_self().get_unique_id();
        if(player_list.get_host().get_socket()->send(packet) != sf::Socket::Done) {
            // TODO: Logging
            std::cout << "Couldn't send list req" << std::endl;
        }
        pending_card_req = true;
    }
}

Play_card* Client_component::receive_card_draw() {
    if(hand_buffer.size() > 0) {
        Play_card* drawn = hand_buffer.back();
        hand_buffer.pop_back();
        is_card_received = true;
        return drawn;
    }
    return nullptr;
}

void Client_component::send_card_play(Play_card* card) {
    sf::Packet packet;
    packet << SIGNAL::WAS_PLAYED_CARD << player_list.get_self().get_unique_id();
    packet << card->color << card->value;
    if(player_list.get_host().get_socket()->send(packet) != sf::Socket::Done) {
        // TODO: Logging
        std::cout << "Couldn't send play card" << std::endl;
    }
}

void Client_component::send_card_play(Guess_card* card) {
    sf::Packet packet;
    packet << SIGNAL::WAS_PLAYED_GUESS << player_list.get_self().get_unique_id();
    packet << card->guess_amount;
    if(player_list.get_host().get_socket()->send(packet) != sf::Socket::Done) {
        // TODO: Logging
        std::cout << "Couldn't send play_check_req" << std::endl;
    }
}

void Client_component::send_rec_all_cards() {
    sf::Packet packet;
    packet << SIGNAL::REC_ALL_CARDS << player_list.get_self().get_unique_id();
    if(player_list.get_host().get_socket()->send(packet) != sf::Socket::Done) {
        // TODO: Logging
        std::cout << "Couldn't send rec all cards" << std::endl;
    }
}

void Client_component::send_msg(std::string msg) {
    sf::Packet packet;
    packet << SIGNAL::CHAT_MSG << player_list.get_self().get_unique_id() << msg;
    if(player_list.get_host().get_socket()->send(packet) != sf::Socket::Done) {
        // TODO: Logging
        std::cout << "Couldn't send msg" << std::endl;
    }
}
