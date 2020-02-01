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

#include "game_manager.h"

Game_manager* Game_manager::instance = nullptr;

Game_manager::Game_manager() {
}

Game_manager* Game_manager::get_instance() {
    if(Game_manager::instance == nullptr) {
        Game_manager::instance = new Game_manager();
    }
    return Game_manager::instance;
}

void Game_manager::reset() {
    delete Game_manager::instance;
    Game_manager::instance = nullptr;
}

void Game_manager::start_game() {
    instance = Game_manager::get_instance();
    instance->state = STATE::ROUND_BEGIN;
}

boost::random_device& Game_manager::get_generator() {
    Game_manager::instance = get_instance();
    return Game_manager::instance->rand_generator;
}

bool Game_manager::check_play(Play_card *card, std::vector<Play_card*>& hand) {
    bool check_ret = false;
    if(Game_manager::get_instance()->state == STATE::PLAY_CARD) {
        if(card->color == card_color::neutral) {
            check_ret = true;
        } else if(Game_manager::get_instance()->dealing_color) {
            bool contains_color = false;
            for(auto hand_card: hand) {
                contains_color |= hand_card->color == *Game_manager::get_instance()->dealing_color;
            }
            // hand contains dealing color thus color has to be dealt
            if(contains_color) {
                check_ret = card->color == *Game_manager::get_instance()->dealing_color;
            } else {
                check_ret = true;
            }
        } else { // No dealing card
            Game_manager::get_instance()->dealing_color = &card->color;
            check_ret = true;
        }
    }
    return check_ret;
}

/**
 * Check if the guess can be played
 * Special rule: total_guess can not equal round number
 */
bool Game_manager::check_play(Guess_card *card) {
    bool check_ret = false;
    if(Game_manager::get_instance()->state == STATE::PLAY_GUESS) {
        // Last to make a guess
        if(Game_manager::get_instance()->is_last) {
            unsigned int tmp = static_cast<unsigned int>(card->guess_amount) + Game_manager::get_instance()->total_guesses;
            check_ret = tmp != Game_manager::get_instance()->round_number;
        } else {
            check_ret = true;
        }

        if(check_ret) {
            Game_manager::get_instance()->played_guess = card;
            Game_manager::get_instance()->total_guesses += static_cast<unsigned int>(card->guess_amount);
        }
    }
    return check_ret;
}

void Game_manager::round_reset() {
    Game_manager* inst = Game_manager::get_instance();
    inst->round_number++;
    inst->total_guesses = 0;
    inst->is_guess_drawn = false;
    inst->is_faceup_drawn = false;
    inst->is_hand_fully_drawn = false;
}
