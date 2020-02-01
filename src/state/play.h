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

#ifndef PLAY_HEADER
#define PLAY_HEADER

#include <SFML/Graphics.hpp>

#include "base_state.h"
#include "gamelogic/play_card.h"
#include "manager/card_manager.h"
#include "manager/game_manager.h"
#include "manager/input_manager.h"
#include "manager/network_manager.h"
#include "userinterface/avatar.h"
#include "userinterface/chat.h"
#include "userinterface/deck.h"
#include "userinterface/placed_card.h"
#include "userinterface/hand.h"
#include "userinterface/score.h"
#include "state/network_issue.h"
#include "state/won.h"
#include "state/lost.h"

template <typename net_comp>
class Play : public Base_state {
    public:
        Play(State_manager& s_m, sf::RenderWindow& window,
             std::unique_ptr<Network_manager<net_comp>>* net, bool replace);
        ~Play();

        void pause();
        void resume();

        void update(const sf::Time& delta_time);
        void draw();

    private:
        std::unique_ptr<Network_manager<net_comp>> network;

        sf::Sprite bg_sprite;
        sf::Texture bg_texture;

        sf::Sprite turn_sprite;
        sf::Texture turn_tex;

        bool is_turn_vis = false;
        bool turn_anim_finished = false;
        sf::Time turn_vis_timer = sf::Time::Zero;
        void turn_anim(const sf::Time& delta_time);
        void reset_turn_anim();

        Deck* deck = nullptr;
        Chat* chat = nullptr;
        Score* score = nullptr;

        Hand<Play_card>* play_hand = nullptr;
        Hand<Guess_card>* guess_hand = nullptr;

        Placed_card* placed_card = nullptr;

        void start_game(const sf::Time& delta_time);
        void avatar_setup();
        void userinterface_setup();

        void switch_state(const sf::Time& delta_time, Game_manager::STATE new_state);

        void draw_cards();

        std::vector<Avatar> player_avatar;

        bool switched_menu = false;

        void set_play_order();
        Play_card* det_trick_winner_card();
        bool winner_det = false;
        Base_player* dec_winner_guess(Play_card* max);
        bool reduced_guess = false;
        void reorder_to_win(Base_player* trick_winner);
        bool reorderd = false;

        Guess_card* rem_guess = nullptr;
        bool is_guess_setup = false;
        void dec_rem_guess();
        int tricks_won = 0;

        void clear_board(bool round_end = false);
        bool cleared_board = false;

        void inc_initial_order();
        bool inc_init_ord = false;
        bool finished_round = false;

        void calc_scores();

        bool has_won();
        void shuffle_deck();


        void handle_text_input(sf::Uint32 code);
        void handle_keyboard_input(sf::Keyboard::Key& key, bool status);
};
#endif
