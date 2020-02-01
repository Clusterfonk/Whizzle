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

#include "card_manager.h"

Card_manager* Card_manager::instance = nullptr;

Card_manager::Card_manager() {
    for(std::vector<Play_card>::iterator it = play_deck.begin(); it != play_deck.end(); it++) {
        deck.push_back(&*(it));
    }
    crt_card = deck.begin();
    crt_guess = guess_deck.begin();
}

Card_manager* Card_manager::get_instance() {
    if(Card_manager::instance == nullptr) {
        Card_manager::instance = new Card_manager();
    }
    return Card_manager::instance;
}

void Card_manager::reset() {
    delete Card_manager::instance;
    Card_manager::instance = nullptr;
}

Play_card* Card_manager::draw_card() {
    Card_manager::get_instance()->card_mutex.lock(); // !Lock
    Play_card* drawn_card = nullptr;
    if(Card_manager::get_instance()->crt_card != Card_manager::get_instance()->deck.end()) {
        drawn_card = *Card_manager::get_instance()->crt_card;
        Card_manager::get_instance()->crt_card++;
    }
    Card_manager::get_instance()->card_mutex.unlock(); // !Unlock
    return drawn_card;
}

Guess_card* Card_manager::draw_guess() {
    Guess_card* drawn_guess = nullptr;
    if(Card_manager::get_instance()->crt_guess != guess_deck.end()) {
        drawn_guess = &(*Card_manager::get_instance()->crt_guess);
        Card_manager::get_instance()->crt_guess++;
        if(drawn_guess) {
            drawn_guess->moveable = true;
        }
    }
    return drawn_guess;
}

void Card_manager::shuffle_deck(boost::random_device& gen) {
    Card_manager::get_instance()->card_mutex.lock(); // !Lock
    std::shuffle(Card_manager::get_instance()->deck.begin(),
                 Card_manager::get_instance()->deck.end(),
                 gen);
    Card_manager::get_instance()->crt_card = Card_manager::get_instance()->deck.begin();
    Card_manager::get_instance()->shuffled = true;
    Card_manager::get_instance()->card_mutex.unlock(); // !Unlock
}

unsigned calc_play_index(card_color color, card_value value) {
    unsigned index = 0;
    switch(color) {
        case blue: {
            index = 0;
            break;
        }
        case red: {
            index = 13;
            break;
        }
        case green: {
            index = 26;
            break;
        }
        case yellow: {
            index = 39;
            break;
        }
        case neutral: {
            index = 52;
            break;
        }
    }
    switch(value) {
        case one: {
            index += 0;
            break;
        }
        case two: {
            index += 1;
            break;
        }
        case three: {
            index += 2;
            break;
        }
        case four: {
            index += 3;
            break;
        }
        case five: {
            index += 4;
            break;
        }
        case six: {
            index += 5;
            break;
        }
        case seven: {
            index += 6;
            break;
        }
        case eight: {
            index += 7;
            break;
        }
        case nine: {
            index += 8;
            break;
        }
        case ten: {
            index += 9;
            break;
        }
        case eleven: {
            index += 10;
            break;
        }
        case twelve: {
            index += 11;
            break;
        }
        case thirteen: {
            index += 12;
            break;
        }
        case narr_1: {
            index += 0;
            break;
        }
        case narr_2: {
            index += 1;
            break;
        }
        case narr_3: {
            index += 2;
            break;
        }
        case narr_4: {
            index += 3;
            break;
        }
        case whizzle_1: {
            index += 4;
            break;
        }
        case whizzle_2: {
            index += 5;
            break;
        }
        case whizzle_3: {
            index += 6;
            break;
        }
        case whizzle_4: {
            index += 7;
            break;
        }
    }
    return index;
}

Play_card* get_play_card(card_color color, card_value value) {
    return &play_deck[calc_play_index(color, value)];
}

Guess_card* get_guess_card(int guess) {
    if(guess < 0) {
        guess = (guess * - 1) + 20;
        assert(guess > 0);
    }
    unsigned index = static_cast<unsigned int>(guess);
    return &guess_deck[index];
}


void Card_manager::round_reset() {
    Card_manager* inst = Card_manager::get_instance();
    inst->crt_guess = guess_deck.begin();
    inst->shuffled = false;
}
