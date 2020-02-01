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

#ifndef CARD_MANAGER_HEADER
#define CARD_MANAGER_HEADER

#include "boost/random.hpp"
#include "boost/random/random_device.hpp"
#include <vector>

#include "gamelogic/guess_card.h"
#include "gamelogic/play_card.h"
#include "userinterface/placed_card.h"
#include "utility/convo.h"

/**
 * Singleton Class manages the card deck across a session
 */
class Card_manager {
	public:
        static Card_manager* get_instance();
        static void reset();

        std::vector<Play_card*> deck;
        static Play_card* draw_card();
        static Guess_card* draw_guess();
        static void shuffle_deck(boost::random_device& gen);
        bool shuffled = false;

        Base_card* last_played_card = nullptr;
        bool can_play_last = false;

        void round_reset();
        std::vector<Play_card*>::iterator crt_card;
        int deck_size = 60;

        sf::Mutex card_mutex;
        static Guess_card* get_guess();
    private:
        Card_manager();

        std::vector<Guess_card>::iterator crt_guess;
        static Card_manager* instance;
};

/**
 *  Reference for all playable cards
 */
std::vector<Play_card> static play_deck {
    Play_card(card_color::blue, card_value::one, sf::IntRect({0, 0}, {103,158})),
    Play_card(card_color::blue, card_value::two, sf::IntRect({103, 0}, {103,158})),
    Play_card(card_color::blue, card_value::three, sf::IntRect({206, 0}, {103,158})),
    Play_card(card_color::blue, card_value::four, sf::IntRect({309, 0}, {103,158})),
    Play_card(card_color::blue, card_value::five, sf::IntRect({412, 0}, {103,158})),
    Play_card(card_color::blue, card_value::six, sf::IntRect({515, 0}, {103,158})),
    Play_card(card_color::blue, card_value::seven, sf::IntRect({618, 0}, {103,158})),
    Play_card(card_color::blue, card_value::eight, sf::IntRect({721, 0}, {103,158})),
    Play_card(card_color::blue, card_value::nine, sf::IntRect({824, 0}, {103,158})),
    Play_card(card_color::blue, card_value::ten, sf::IntRect({927, 0}, {103,158})),
    Play_card(card_color::blue, card_value::eleven, sf::IntRect({1030, 0}, {103,158})),
    Play_card(card_color::blue, card_value::twelve, sf::IntRect({1133, 0}, {103,158})),
    Play_card(card_color::blue, card_value::thirteen, sf::IntRect({1236, 0}, {103,158})),
    Play_card(card_color::red, card_value::one, sf::IntRect({721, 158}, {103,158})),
    Play_card(card_color::red, card_value::two, sf::IntRect({824, 158}, {103,158})),
    Play_card(card_color::red, card_value::three, sf::IntRect({927, 158}, {103,158})),
    Play_card(card_color::red, card_value::four, sf::IntRect({1030, 158}, {103,158})),
    Play_card(card_color::red, card_value::five, sf::IntRect({1133, 158}, {103,158})),
    Play_card(card_color::red, card_value::six, sf::IntRect({1236, 158}, {103,158})),
    Play_card(card_color::red, card_value::seven, sf::IntRect({1339, 158}, {103,158})),
    Play_card(card_color::red, card_value::eight, sf::IntRect({1442, 158}, {103,158})),
    Play_card(card_color::red, card_value::nine, sf::IntRect({1545, 158}, {103,158})),
    Play_card(card_color::red, card_value::ten, sf::IntRect({1648, 158}, {103,158})),
    Play_card(card_color::red, card_value::eleven, sf::IntRect({1751, 158}, {103,158})),
    Play_card(card_color::red, card_value::twelve, sf::IntRect({1854, 158}, {103,158})),
    Play_card(card_color::red, card_value::thirteen, sf::IntRect({0, 316}, {103,158})),
    Play_card(card_color::green, card_value::one, sf::IntRect({1339, 0}, {103,158})),
    Play_card(card_color::green, card_value::two, sf::IntRect({1442, 0}, {103,158})),
    Play_card(card_color::green, card_value::three, sf::IntRect({1545, 0}, {103,158})),
    Play_card(card_color::green, card_value::four, sf::IntRect({1648, 0}, {103,158})),
    Play_card(card_color::green, card_value::five, sf::IntRect({1751, 0}, {103,158})),
    Play_card(card_color::green, card_value::six, sf::IntRect({1854, 0}, {103,158})),
    Play_card(card_color::green, card_value::seven, sf::IntRect({0, 158}, {103,158})),
    Play_card(card_color::green, card_value::eight, sf::IntRect({103, 158}, {103,158})),
    Play_card(card_color::green, card_value::nine, sf::IntRect({206, 158}, {103,158})),
    Play_card(card_color::green, card_value::ten, sf::IntRect({309, 158}, {103,158})),
    Play_card(card_color::green, card_value::eleven, sf::IntRect({412, 158}, {103,158})),
    Play_card(card_color::green, card_value::twelve, sf::IntRect({515, 158}, {103,158})),
    Play_card(card_color::green, card_value::thirteen, sf::IntRect({618, 158}, {103,158})),
    Play_card(card_color::yellow, card_value::one, sf::IntRect({103, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::two, sf::IntRect({206, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::three, sf::IntRect({309, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::four, sf::IntRect({412, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::five, sf::IntRect({515, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::six, sf::IntRect({618, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::seven, sf::IntRect({721, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::eight, sf::IntRect({824, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::nine, sf::IntRect({927, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::ten, sf::IntRect({1030, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::eleven, sf::IntRect({1133, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::twelve, sf::IntRect({1236, 316}, {103,158})),
    Play_card(card_color::yellow, card_value::thirteen, sf::IntRect({1339, 316}, {103,158})),
    Play_card(card_color::neutral, card_value::narr_1, sf::IntRect({1442, 316}, {103,158})),
    Play_card(card_color::neutral, card_value::narr_2, sf::IntRect({1442, 316}, {103,158})),
    Play_card(card_color::neutral, card_value::narr_3, sf::IntRect({1442, 316}, {103,158})),
    Play_card(card_color::neutral, card_value::narr_4, sf::IntRect({1442, 316}, {103,158})),
    Play_card(card_color::neutral, card_value::whizzle_1, sf::IntRect({1545, 316}, {103,158})),
    Play_card(card_color::neutral, card_value::whizzle_2, sf::IntRect({1545, 316}, {103,158})),
    Play_card(card_color::neutral, card_value::whizzle_3, sf::IntRect({1545, 316}, {103,158})),
    Play_card(card_color::neutral, card_value::whizzle_4, sf::IntRect({1545, 316}, {103,158}))
};

/**
  * Reference all playable Guess cards
  */
std::vector<Guess_card> static guess_deck {
    Guess_card(0, sf::IntRect({0, 0}, {103, 158})),
    Guess_card(1, sf::IntRect({103, 0}, {103, 158})),
    Guess_card(2, sf::IntRect({0, 158}, {103, 158})),
    Guess_card(3, sf::IntRect({103, 158}, {103, 158})),
    Guess_card(4, sf::IntRect({0, 316}, {103, 158})),
    Guess_card(5, sf::IntRect({103, 316}, {103, 158})),
    Guess_card(6, sf::IntRect({0, 474}, {103, 158})),
    Guess_card(7, sf::IntRect({103, 474}, {103, 158})),
    Guess_card(8, sf::IntRect({0, 632}, {103, 158})),
    Guess_card(9, sf::IntRect({103, 632}, {103, 158})),
    Guess_card(10, sf::IntRect({0, 790}, {103, 158})),
    Guess_card(11, sf::IntRect({103, 790}, {103, 158})),
    Guess_card(12, sf::IntRect({0, 948}, {103, 158})),
    Guess_card(13, sf::IntRect({103, 948}, {103, 158})),
    Guess_card(14, sf::IntRect({0, 1106}, {103, 158})),
    Guess_card(15, sf::IntRect({103, 1106}, {103, 158})),
    Guess_card(16, sf::IntRect({0, 1264}, {103, 158})),
    Guess_card(17, sf::IntRect({103, 1264}, {103, 158})),
    Guess_card(18, sf::IntRect({0, 1422}, {103, 158})),
    Guess_card(19, sf::IntRect({103, 1422}, {103, 158})),
    Guess_card(20, sf::IntRect({0, 1580}, {103, 158})),
    Guess_card(-1, sf::IntRect({0, 0}, {103, 158})),
    Guess_card(-2, sf::IntRect({103, 0}, {103, 158})),
    Guess_card(-3, sf::IntRect({0, 158}, {103, 158})),
    Guess_card(-4, sf::IntRect({103, 158}, {103, 158})),
    Guess_card(-5, sf::IntRect({0, 316}, {103, 158})),
    Guess_card(-6, sf::IntRect({103, 316}, {103, 158})),
    Guess_card(-7, sf::IntRect({0, 474}, {103, 158})),
    Guess_card(-8, sf::IntRect({103, 474}, {103, 158})),
    Guess_card(-9, sf::IntRect({0, 632}, {103, 158})),
    Guess_card(-10, sf::IntRect({103, 632}, {103, 158})),
    Guess_card(-11, sf::IntRect({0, 790}, {103, 158})),
    Guess_card(-12, sf::IntRect({103, 790}, {103, 158})),
    Guess_card(-13, sf::IntRect({0, 948}, {103, 158})),
    Guess_card(-14, sf::IntRect({103, 948}, {103, 158})),
    Guess_card(-15, sf::IntRect({0, 1106}, {103, 158})),
    Guess_card(-16, sf::IntRect({103, 1106}, {103, 158})),
    Guess_card(-17, sf::IntRect({0, 1264}, {103, 158})),
    Guess_card(-18, sf::IntRect({103, 1264}, {103, 158})),
    Guess_card(-19, sf::IntRect({0, 1422}, {103, 158})),
    Guess_card(-20, sf::IntRect({103, 1422}, {103, 158}))
};

unsigned calc_play_index(card_color color, card_value value);
Play_card* get_play_card(card_color color, card_value value);
Guess_card* get_guess_card(int guess);

#endif
