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

#include "host_component.h"
#include <iostream>

sf::TcpListener* Host_component::tcp_listener = nullptr;

Host_component::Host_component() : Base_network_component() {
    //ip_addr = sf::IpAddress::getPublicAddress();
    ip_addr = sf::IpAddress("127.0.0.1"); // !DEBUG
    player_list.emplace_back(new Local_player(uuids_gen(), "Player 1"));
    l_thread = new std::thread(&Host_component::listen, this);
}

Host_component::~Host_component() {
    disconnect();

    // Stopping l_thread
    is_listening = false;
    l_thread->join();

    tcp_listener->close();

    player_list.clear();
    selector.clear();
}

/**
 * Sends every player a signal to disconnect 
 */
void Host_component::disconnect() {
    sf::Packet packet;
    packet << SIGNAL::HOST_DISCONNECTED;
    enemy_mutex.lock(); //!Lock
    Player_list::Iterator<Remote_player> itr = player_list.begin();
    while(itr != player_list.end()) {
        std::cout << "Disconnecting player: " << itr->get_name() << std::endl;
        if(itr->get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Couldn't send disconnect message" << std::endl;
            if(is_session_started) {
                connection_issue = true;
                connection = false;
                enemy_mutex.unlock();
                return;
            }
        } else {
            selector.remove(*itr->get_socket());
            player_list.erase(itr);
            std::cout << "Player disconnected " << player_list.size() << std::endl;
        }
        itr = player_list.begin();
    }
    enemy_mutex.unlock(); //!Unlock
    connection = false;
}

/**
 * Disconnects player at given position in the enemy_list
 */
void Host_component::disconnect(Player_list::Iterator<Remote_player> itr) {
        sf::Packet packet;
        packet << SIGNAL::HOST_DISCONNECTED;
        if(itr->get_socket()->send(packet) != sf::Socket::Done){
            if(is_session_started) {
                connection_issue = true;
            }
            std::cout << "Couldn't send disconnect message" << std::endl;
        }
        selector.remove(*itr->get_socket());
        player_list.erase(itr);
        std::cout << "Player disconnected" << player_list.size() << std::endl;
        if(is_session_started) {
            broadcast_issue();
        }
}

/**
 * Processes packets contained in the queue
 */
void Host_component::update(const sf::Time& delta_time) {
    if (receive_queue.size() > 0) {
        packet_mutex.lock(); // !Lock
        sf::Packet rec_packet = receive_queue.front();
        SIGNAL signal;
        boost::uuids::uuid enemy_id;
        rec_packet >> signal >> enemy_id;
        enemy_mutex.lock(); // !Lock

        switch (signal) {
            case SIGNAL::PLAYER_DISCONNECTED:
                {
                    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
                        itr != player_list.end(); itr++) {
                        if(itr->get_unique_id() == enemy_id) {
                            Notification_bubble::disc(itr->get_name());
                            selector.remove(*itr->get_socket());
                            player_list.erase(itr);
                            std::cout << "Player disconnected " << player_list.size() << std::endl;
                            if(is_session_started) {
                                broadcast_issue();
                            }
                            break;
                        }
                    }
                    break;
                }
            case SIGNAL::PING:
                {
                    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
                        itr != player_list.end(); itr++) {
                        if(itr->get_unique_id() == enemy_id) {
                            itr->ping_timer = sf::Time::Zero;
                            itr->pending_ping = false;
                            break;
                        }
                    }
                    break;
                }
            case SIGNAL::PLAYER_READY:
                {
                    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
                        itr != player_list.end(); itr++) {
                        if(itr->get_unique_id() == enemy_id) {
                            itr->is_ready = true;
                            break;
                        }
                    }
                    break;
                }
            case SIGNAL::PLAYER_LIST_REQ:
                {
                    send_player_list(enemy_id);
                    break;
                }
            case SIGNAL::PLAY_CARD_REQ:
                {

                    Card_manager::get_instance()->card_mutex.lock(); // !Lock
                    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
                        itr != player_list.end(); itr++) {
                        if(itr->get_unique_id() == enemy_id) {
                            Play_card* req_card = Card_manager::draw_card();
                            if(req_card) {
                                sf::Packet packet;
                                packet << SIGNAL::PLAY_CARD << req_card->color << req_card->value;
                                if(itr->get_socket()->send(packet) != sf::Socket::Done){
                                    // TODO: Logging
                                    std::cout << "Couldn't send card" << std::endl;
                                }
                            }
                            break;
                        }
                    }
                    Card_manager::get_instance()->card_mutex.unlock(); // !Unlock
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
                    broadcast_play(played_card, enemy_id);
                    break;
                }
            case SIGNAL::WAS_PLAYED_GUESS:
                {
                    int guess;
                    rec_packet >> guess;

                    Guess_card* played_guess = get_guess_card(guess);
                    player_on_turn->played_guess = played_guess;
                    Game_manager::get_instance()->total_guesses += static_cast<unsigned int>(guess);
                    Game_manager::get_instance()->is_last = next_players_turn();
                    broadcast_guess(played_guess, enemy_id);
                    break;
                }
            case SIGNAL::REC_ALL_CARDS:
                {
                    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
                        itr != player_list.end(); itr++) {
                        if(itr->get_unique_id() == enemy_id) {
                            itr->rec_cards = true;
                        }
                    }
                    break;
                }
             case SIGNAL::CHAT_MSG:
                    {
                        std::string msg;
                        rec_packet >> msg;

                        broadcast_msg(msg, enemy_id);

                        if(chat) {
                            for(Player_list::Iterator<Base_player> itr = player_list.begin();
                                itr != player_list.end(); itr++) {
                                if(itr->get_unique_id() == enemy_id) {
                                    chat->add_msg("[" + itr->get_name() + "]: " + msg);
                                }
                            }
                        }
                        break;
                    }
            }
        enemy_mutex.unlock(); // !Unlock
        receive_queue.pop();
        packet_mutex.unlock(); // !Unlock
    }
    ping_update(delta_time);
}

/**
 * Returns the base64_encoded ip_addr
 * Using non standardized alphabet
 */
std::string Host_component::get_ip_addr() {
    return base64_encode(ip_addr);
}

/**
 *  Check if Host can start the session
 */
bool Host_component::start_play() {
    if(player_list.size() >= 1 ) { //DEBUG ONLY! otherwise 3
        max_connection_count = player_list.size(); // prevent others to join
        return send_session_play();
    } else {
        return false;
    }
}

/**
 * Listen to incoming connections and packets
 */
void Host_component::listen() {
    if (Host_component::tcp_listener == nullptr) {
        Host_component::tcp_listener = new sf::TcpListener();
    }
    if (Host_component::tcp_listener->listen(port) != sf::Socket::Done) {
        std::cout << "Couldn't listen to port" << std::endl;
        return;
    }
    std::cout << "listening thread started" << std::endl;
    selector.add(*Host_component::tcp_listener);
    std::unique_ptr<sf::TcpSocket> inc_socket;
    connection = true;
    is_listening = true;
    while(is_listening) {
        if(selector.wait(sf::milliseconds(32))) {
            // tcp_listener awaits connection
            if(selector.isReady(*Host_component::tcp_listener)) {
                // Socket that waits for new connections
                inc_socket = std::make_unique<sf::TcpSocket>();
                if(Host_component::tcp_listener->accept(*inc_socket) == sf::Socket::Done) {
                    if (player_list.size() < max_connection_count) { // Session not full
                        enemy_mutex.lock(); // !Lock
                        player_list.emplace_back(new Remote_player(&inc_socket, uuids_gen()));
                        selector.add(*player_list.back<Remote_player>().get_socket());
                        send_connection_info(player_list.back<Remote_player>());
                        player_list.back<Remote_player>().set_name("Player " + std::to_string(player_list.size()));
                        Notification_bubble::notify(player_list.back<Remote_player>().get_name());
                        enemy_mutex.unlock(); // !Unlock
                    } else { // Session full
                        enemy_mutex.lock(); //!Lock
                        send_session_full(*inc_socket);
                        enemy_mutex.unlock(); //!Unlock
                        std::cout << "Session Full disconnecting" << std::endl;
                    }
                }
                // Listener is busy, check clients for incomming packets
            } else {
                packet_mutex.lock(); // !Lock
                enemy_mutex.lock(); // !Lock
                for(Player_list::Iterator<Remote_player> itr = player_list.begin();
                    itr != player_list.end(); itr++) {
                    if(selector.isReady(*itr->get_socket())) {
                        sf::Packet inc_packet;
                        // Client sent data
                        if(itr->get_socket()->receive(inc_packet) == sf::Socket::Done) {
                            receive_queue.push(inc_packet);
                        }
                    }
                }
                enemy_mutex.unlock(); // !Unlock
                packet_mutex.unlock(); // !Unlock
            }
        }
    }
    std::cout << "listening thread stopped" << std::endl;
}

/**
 * Sends a player the host uuid and his connection information (id, name)
 * SIGNAL << HOST_UUID << HOST_NAME << PLAYER_UUID << PLAYER_NAME
 */
void Host_component::send_connection_info(Remote_player& enemy) {
    sf::Packet packet;
    enemy.set_name("Player " + std::to_string(player_list.size()));
    packet << SIGNAL::PLAYER_INFO << player_list.get_self().get_unique_id() <<
              player_list.get_self().get_name() <<
              enemy.get_unique_id() <<
              enemy.get_name();
    if(enemy.get_socket()->send(packet) != sf::Socket::Done) {
        std::cout << "Couldn't send connection_id packet" << std::endl;
    }
}

/**
 * Sends a player attempting to join the session that it exeeds max_connection_count
 */
void Host_component::send_session_full(sf::TcpSocket& socket) {
    sf::Packet packet;
    packet << SIGNAL::SESSION_FULL;
    if(socket.send(packet) != sf::Socket::Done) {
        std::cout << "Couldn't send session_full" << std::endl;
    }
}

/**
 * Reply on a ping from an enemy
 */
void Host_component::send_ping_req(Remote_player& enemy) {
    sf::Packet packet;
    packet << SIGNAL::PING_REQ;
    if(enemy.get_socket()->send(packet) != sf::Socket::Done) {
        std::cout << "Couldn't respond to ping" << std::endl;
        return;
    }
    enemy.pending_ping = true;
}

/**
 * Advance ping timer for all enemys and timeout the ones exeeding the timeout_limit
 */
void Host_component::ping_update(const sf::Time& delta_time) {
    enemy_mutex.lock(); // !Lock
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        itr->ping_timer += delta_time;
        if(!itr->pending_ping) {
            if(itr->ping_timer >= sf::seconds(1.0f)) {
                send_ping_req(*itr);
            }
        }
        if(itr->ping_timer >= ping_time_limit) {
            std::cout << "ping timeout" << std::endl;
            disconnect(itr);
        }
    }
    enemy_mutex.unlock(); // !Unlock
}

/**
 * Sends all enemys that the session started, aborts if someone couldn't get the signal
 */
bool Host_component::send_session_play() {
    enemy_mutex.lock(); // !Lock
    sf::Packet packet;
    packet << SIGNAL::SESSION_PLAY;
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(itr->get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Couldn't send Session play signal" << std::endl;
            enemy_mutex.unlock(); // !Unlock
            return false;
        }
    }
    enemy_mutex.unlock(); // !Unlock
    is_session_started = true;
    return true;
}


/**
 * Sends a spezific player the player_list or broadcasts it to all connected players
 */
void Host_component::send_player_list(boost::uuids::uuid spez_id) {
    enemy_mutex.lock(); // !Lock
    sf::Packet packet;
    packet << SIGNAL::PLAYER_LIST;
    packet << player_list.size();
    for(Player_list::Iterator<Base_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        packet << itr->get_unique_id();
        packet << itr->get_name();
    }
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(spez_id.is_nil() || itr->get_unique_id() == spez_id) {
            if(itr->get_socket()->send(packet) != sf::Socket::Done) {
                std::cout << "Could not send player_list to " << itr->get_name() << std::endl;
            }
        }
    }
    player_on_turn = player_list.begin();
    enemy_mutex.unlock(); // !Unlock
}

/**
 * Shuffle the player list and set the order in which is played
 */
void Host_component::shuffle_player_list() {
    enemy_mutex.lock(); // !Lock
    player_list.shuffle(Game_manager::get_generator());
    for(Player_list::Iterator<Base_player> itr = player_list.begin();
        itr != player_list.end();
        itr++) {
        Game_manager::get_instance()->initial_play_order.push_back(itr->get_unique_id());
    }
    Game_manager::get_instance()->i_p_o_itr = Game_manager::get_instance()->initial_play_order.begin();
    enemy_mutex.unlock(); // !Unlock
}

/**
 * Check if all players are ready
 */
bool Host_component::check_players_ready() {
    bool ret = true;
    enemy_mutex.lock(); // !Lock
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        ret &= itr->is_ready;
    }
    enemy_mutex.unlock(); // !Unlock

    if(ret) {
        reset_ready();
    }
    return ret;
}

/**
 * Resets ready status of all players
 */
void Host_component::reset_ready() {
    enemy_mutex.lock(); // !Lock
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        itr->is_ready = false;
    }
    enemy_mutex.unlock(); // !Unlock
}

/**
 * Send the State to all players
 */
void Host_component::send_next_state(Game_manager::STATE state) {
    sf::Packet packet;
    packet << SIGNAL::NEXT_STATE << state;
    enemy_mutex.lock(); // !Lock
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(itr->get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Could not send next state to " << itr->get_name() << std::endl;
        }
    }
    enemy_mutex.unlock(); // !Unlock
    Game_manager::get_instance()->state = state;
}

/**
 * Broadcast the facup card of the current round
 */
void Host_component::broadcast_faceup(Play_card *card) {
    sf::Packet packet;
    if(card) {
        packet << SIGNAL::FACEUP_CARD << true << card->color << card->value;
    } else {
        packet << SIGNAL::FACEUP_CARD << false;
    }
    enemy_mutex.lock(); // !Lock
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(itr->get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Could not send faceup card to " << itr->get_name() << std::endl;
        }
    }
    enemy_mutex.unlock(); // !Unlock
}

/**
 * Broadcast that there is an issue with the network connection
 */
void Host_component::broadcast_issue() {
    sf::Packet packet;
    packet << SIGNAL::CONNECTION_ISSUE;
    enemy_mutex.lock(); //!Lock
    Player_list::Iterator<Remote_player> itr = player_list.begin();
    while(itr != player_list.end()) {
        if(itr->get_socket()->send(packet) == sf::Socket::Done) {
            selector.remove(*itr->get_socket());
            player_list.erase(itr);
        }
        itr = player_list.begin();
    }
    enemy_mutex.unlock(); //!Unlock
    connection = false;
    connection_issue = true;
}

/**
 * Broadcasts a play to all players
 */
void Host_component::broadcast_play(Play_card* card) {
    if(!card) return;
    sf::Packet packet;
    packet << SIGNAL::WAS_PLAYED_CARD << card->color << card->value;
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(itr->get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Could not send card to " << itr->get_name() << std::endl;
        }
    }
}

/**
 * Broadcasts a guess to all players
 */
void Host_component::broadcast_guess(Guess_card* guess) {
    if(!guess) return;
    sf::Packet packet;
    packet << SIGNAL::WAS_PLAYED_GUESS << guess->guess_amount;
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(itr->get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Could not send guess to " << itr->get_name() << std::endl;
        }
    }
}


/**
 * Broadcasts a play to all players except the sending player
 */
void Host_component::broadcast_play(Play_card* card, boost::uuids::uuid except_id) {
    if(!card) return;
    sf::Packet packet;
    packet << SIGNAL::WAS_PLAYED_CARD << card->color << card->value;
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(itr->get_unique_id() != except_id) {
            if(itr->get_socket()->send(packet) != sf::Socket::Done) {
                std::cout << "Could not send card to " << itr->get_name() << std::endl;
            }
        }
    }
}

/**
 * Broadcasts a guess to all players except the sending player
 */
void Host_component::broadcast_guess(Guess_card* guess, boost::uuids::uuid except_id) {
    if(!guess) return;
    sf::Packet packet;
    packet << SIGNAL::WAS_PLAYED_GUESS << guess->guess_amount;
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(itr->get_unique_id() != except_id) {
            if(itr->get_socket()->send(packet) != sf::Socket::Done) {
                std::cout << "Could not send guess to " << itr->get_name() << std::endl;
            }
        }
    }
}

/**
 * Broadcasts current deck_size to all players
 */
void Host_component::broadcast_deck_size(int deck_size) {
    sf::Packet packet;
    packet << SIGNAL::DECK_SIZE << deck_size;
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
            if(itr->get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Could not send guess to " << itr->get_name() << std::endl;
            }
    }
}

/**
 * Broadcasts a msg to all players except the player that send the msg
 */
void Host_component::broadcast_msg(std::string msg, boost::uuids::uuid sender_id) {
    sf::Packet packet;
    packet << SIGNAL::CHAT_MSG << msg << sender_id;
    for(Player_list::Iterator<Remote_player> itr = player_list.begin();
        itr != player_list.end(); itr++) {
        if(itr->get_unique_id() != sender_id) {
            if(itr->get_socket()->send(packet) != sf::Socket::Done) {
            std::cout << "Could not send msg to " << itr->get_name() << std::endl;
            }
        }
    }
}
