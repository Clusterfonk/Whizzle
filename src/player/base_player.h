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

#ifndef BASE_PLAYER_HEADER
#define BASE_PLAYER_HEADER

#include <boost/uuid/uuid.hpp>
#include <SFML/Network.hpp>

#include "gamelogic/play_card.h"
#include "gamelogic/guess_card.h"

/**
 * Abstract class representing the base_player
 */
class Base_player : sf::NonCopyable {
	public:
        Base_player(boost::uuids::uuid id);
        Base_player(boost::uuids::uuid id, std::string name);
        Base_player();
        virtual ~Base_player();
        Base_player(const Base_player&) = delete;

        int score = 0;
        bool is_ready = false;

        bool is_on_turn = false;
        Play_card* played_card = nullptr;
        Guess_card* played_guess = nullptr;

        bool rec_cards = false;

        virtual std::string get_name() final;
        virtual void set_name(std::string player_name) final;

        virtual boost::uuids::uuid get_unique_id() final;
        virtual void set_unique_id(boost::uuids::uuid id) final;
    private:
        boost::uuids::uuid unique_id;
    protected:
        std::string name = "Player X";
};
#endif
