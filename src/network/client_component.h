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

#ifndef CLIENT_COMPONENT_HEADER 
#define CLIENT_COMPONENT_HEADER 

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include "base_network_component.h"
#include "gamelogic/play_card.h"
#include "manager/card_manager.h"
#include "utility/convo.h"

class Client_component : public Base_network_component {
    public:
        Client_component();
        ~Client_component();

        bool connect(std::string ip_encoded);
        void disconnect();

        void update(const sf::Time& delta_time);

        void receive();
        void send_player_list_req();
        void send_ready_play();
        sf::Time player_list_rec_timer = sf::seconds(1.0f);

        void send_card_request();
        Play_card* receive_card_draw();

        void send_card_play(Play_card* card);
        void send_card_play(Guess_card* card);

        void send_rec_all_cards();

        void send_msg(std::string msg);
    public:
        bool is_player_list_received = false;
        bool is_card_received = false;
        std::vector<Play_card*> hand_buffer;

    private:
        void reset_component(bool clean = true);
        void force_disconnect();

        void send_ping();
        sf::Time ping_time = sf::Time::Zero;
        bool is_pinging = false;

        bool is_ready = false;

        bool pending_card_req = false;

};
#endif
