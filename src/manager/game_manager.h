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

#ifndef GAME_MANAGER_HEADER
#define GAME_MANAGER_HEADER

#include <SFML/Graphics/Rect.hpp>
#include <boost/random/random_device.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>

#include "gamelogic/play_card.h"
#include "gamelogic/guess_card.h"

/**
 * Singleton Class used for managing the gameflow
 */
class Game_manager {
public:
    static Game_manager* get_instance();
    static void start_game();

    enum class STATE {
        GAME_BEGIN,
        ROUND_BEGIN,
        PLAY_GUESS,
        PLAY_CARD,
        TRICK_END,
        ROUND_END,
        GAME_END
    };

    unsigned int round_number = 1;

    STATE state = STATE::GAME_BEGIN;
    bool is_player_setup = false;
    bool is_avatar_setup = false;
    bool is_userinterface_setup = false;
    bool is_hand_fully_drawn = false;
    bool is_guess_drawn = false;
    unsigned int total_guesses = 0;
    static boost::random_device& get_generator();

    static bool check_play(Play_card* card, std::vector<Play_card*>& hand);
    static bool check_play(Guess_card* card);

    static void reset();

    std::vector<boost::uuids::uuid> initial_play_order;
    std::vector<boost::uuids::uuid>::iterator i_p_o_itr = initial_play_order.begin();

    Play_card* faceup_card = nullptr;
    bool is_faceup_drawn = false;
    Guess_card* played_guess = nullptr;

    card_color* trump_color = nullptr;
    card_color* dealing_color = nullptr;

    bool is_last = false;
    bool is_play_order_setup = false;
    static void round_reset();
    int winner_score;
private:
    Game_manager();

    boost::random_device rand_generator;

    static Game_manager* instance;
};
#endif
