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

#ifndef HOST_COMPONENT_HEADERS
#define HOST_COMPONENT_HEADERS

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <thread>
#include <queue>

#include "base_network_component.h"
#include "manager/card_manager.h"
#include "manager/game_manager.h"
#include "userinterface/notification_bubble.h"
#include "utility/convo.h"


class Host_component : public Base_network_component {
    public:
        Host_component();
        ~Host_component();

        void disconnect();

        void update(const sf::Time& delta_time);

        std::string get_ip_addr();

        bool start_play();
        void send_player_list(boost::uuids::uuid spez_id = boost::uuids::nil_uuid());
        void shuffle_player_list();

        bool check_players_ready();
        void send_next_state(Game_manager::STATE state);

        void broadcast_faceup(Play_card* card);

        void broadcast_play(Play_card* card);
        void broadcast_play(Play_card* card, boost::uuids::uuid except_id);
        void broadcast_guess(Guess_card* guess);
        void broadcast_guess(Guess_card* guess, boost::uuids::uuid except_id);

        void broadcast_deck_size(int deck_size);
        void broadcast_msg(std::string msg, boost::uuids::uuid sender_id);

    public :
        bool is_listening = false;
        size_t max_connection_count = 6;
    private:
        void listen();
        void send_connection_info(Remote_player& enemy);
        void send_session_full(sf::TcpSocket& socket);
        void send_ping_req(Remote_player& enemy);
        void ping_update(const sf::Time& delta_time);
        void disconnect(Player_list::Iterator<Remote_player> itr);
        bool send_session_play();
        void send_guess_req();
        void reset_ready();
        void broadcast_issue();

    private:
        boost::uuids::random_generator uuids_gen;

        sf::IpAddress ip_addr;
        sf::SocketSelector selector;

        std::thread* l_thread;
        static sf::TcpListener* tcp_listener;

        sf::Mutex packet_mutex;
        std::queue<sf::Packet> receive_queue;

        unsigned int ready_count = 0;
};
#endif
