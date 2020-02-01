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

#ifndef HOST_COMPONENT_HEADER 
#define HOST_COMPONENT_HEADER 

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Network.hpp>
#include "avatar.h"

#include "network/base_network_component.h"
#include "network/client_component.h"
#include "network/host_component.h"

#include "userinterface/hand.h"

template<typename Derived>
class Network_manager {
    public:
        Network_manager();
        ~Network_manager();

        void connect();
        void disconnect();
        void update(const sf::Time& delta_time);

        void create_avatar(std::vector<Avatar>& avatar_list);

        bool is_connected();
        bool has_issue();
        size_t get_connection_count();

        void next_players_turn();
        bool is_my_turn();

        void reorder_play_list(boost::uuids::uuid winner_id);

        void calc_deck_size(const sf::Time& delta_time);
        int send_rec = 0;

        Play_card* draw_card();
        Play_card* draw_faceup();
        bool all_cards_dealt();
        bool send_all_cards_rec = false;

        bool send_play(Base_card* played, std::vector<Play_card*>& hand);
        void send_next_state(Game_manager::STATE state);

        void send_msg(std::string msg);
    public:
        Derived component;

    private:
        sf::Time deck_size_update = sf::Time::Zero;

};
#endif
