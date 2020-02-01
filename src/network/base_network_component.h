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

#ifndef BASE_NETWORK_COMPONENT_HEADER
#define BASE_NETWORK_COMPONENT_HEADER

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Network.hpp>

#include "player/remote_player.h"
#include "player/local_player.h"
#include "utility/player_list.h"
#include "userinterface/chat.h"

/**
 * Enum representing signals used to send/receive
 * prevent implicit integer conversion
 */
enum class SIGNAL {
    SESSION_FULL = -1,
    PING = 0,
    PING_REQ,
    PLAYER_INFO,
    PLAYER_LIST,
    PLAYER_LIST_REQ,
    GUESS_REQ,
    PLAYER_GUESS,
    REC_ALL_CARDS,
    PLAY_CARD_REQ,
    PLAY_CARD,
    FACEUP_CARD,
    PLAYER_DISCONNECTED,
    PLAYER_RECONNECTED,
    PLAYER_READY,
    HOST_DISCONNECTED,
    CONNECTION_ISSUE,
    SESSION_PLAY,
    NEXT_STATE,
    PLAY_CHECK_REQ,
    GUESS_CHECK_REQ,
    WAS_PLAYED_CARD,
    WAS_PLAYED_GUESS,
    PLAYERS_TURN,
    CHECK_RES,
    DECK_SIZE,
    CHAT_MSG
};

class Base_network_component : sf::NonCopyable {
    public:
        Base_network_component();
        virtual ~Base_network_component() = default;

        virtual void disconnect() = 0;

        virtual void update(const sf::Time& delta_time) = 0;

        bool is_connected();
        bool has_issue();
        size_t get_connection_count();

        Player_list player_list;
        sf::Mutex enemy_mutex;

        Player_list::Iterator<Base_player> player_on_turn = player_list.end();
        bool next_players_turn();
        bool is_session_started = false;

        void reorder(boost::uuids::uuid winner_id);

        void set_chat(Chat* ref_chat);

    protected:
        const unsigned short port = 8000;

        bool connection = false;
        bool connection_issue = false;

        sf::Time ping_time_limit = sf::seconds(8.0f);

        Chat* chat;
};
#endif
