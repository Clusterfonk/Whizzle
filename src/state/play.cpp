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

#include "play.h"

namespace  {
    namespace userinterface {
        int progress = 0;
    }
    namespace timer {
        sf::Time recheck = sf::Time::Zero;
    }
}

template<typename net_comp>
Play<net_comp>::Play(State_manager& s_m, sf::RenderWindow& window,
    std::unique_ptr<Network_manager<net_comp>>* net, bool replace) :
    Base_state(s_m, window, replace) {
    if(net) {
        network = std::move(*net);
    }
    bg_texture = Resource_manager::get_texture("../assets/play_base.png");
    bg_sprite.setTexture(bg_texture);

    turn_tex = Resource_manager::get_texture("../assets/your_turn.png");
    turn_sprite.setTexture(turn_tex);
    turn_sprite.setPosition(1280.0f / 2.0f - turn_sprite.getGlobalBounds().width / 2.0f,
                            720.0f / 2.0f - turn_sprite.getGlobalBounds().height / 2.0f);

    sf::Color turn_color = turn_sprite.getColor();
    turn_color.a = 0;
    turn_sprite.setColor(turn_color);
    placed_card = new Placed_card({1280 / 2, 720 / 2});
    play_hand = new Hand<Play_card>(725);
    guess_hand = new Hand<Guess_card>(665);
}

template <>
void Play<Host_component>::switch_state(const sf::Time& delta_time, Game_manager::STATE new_state) {
    if(!network) return;
    if(timer::recheck == sf::Time::Zero) {
        if(network->component.check_players_ready()) {
        network->component.send_next_state(new_state);
        }
    }
    timer::recheck += delta_time;
    if(timer::recheck >= sf::seconds(1)) {
        timer::recheck = sf::Time::Zero;
    }
}

template <>
void Play<Client_component>::switch_state(const sf::Time& delta_time, Game_manager::STATE new_state) {
    if(!network) return;
    network->component.send_ready_play();
}

template <>
void Play<Host_component>::shuffle_deck() {
    Card_manager::shuffle_deck(Game_manager::get_generator());
}

template <>
void Play<Client_component>::shuffle_deck() {
}


template <>
void Play<Host_component>::start_game(const sf::Time& delta_time) {
    if(!network) return;
    if(!Game_manager::get_instance()->is_player_setup) {
        network->component.shuffle_player_list();
        network->component.send_player_list();
        network->component.player_on_turn = network->component.player_list.begin();
        Player_list::Iterator<Base_player> tmp = network->component.player_on_turn;
        tmp++;
        Game_manager::get_instance()->is_last = network->component.player_on_turn != network->component.player_list.end() &&
                tmp == network->component.player_list.end();
        Game_manager::get_instance()->is_player_setup = true;
    } else {
        if(Game_manager::get_instance()->is_avatar_setup) {
            userinterface_setup();
        } else {
            avatar_setup();
        }
    }

    // Shuffle Deck
    if(!Card_manager::get_instance()->shuffled) {
        shuffle_deck();
    }

    if(Game_manager::get_instance()->is_userinterface_setup &&
            Game_manager::get_instance()->is_avatar_setup && Card_manager::get_instance()->shuffled) {
        switch_state(delta_time, Game_manager::STATE::ROUND_BEGIN);
    }
}

template <>
void Play<Client_component>::start_game(const sf::Time& delta_time) {
    if(!network) return;
    userinterface_setup();
    if(Game_manager::get_instance()->is_player_setup && Game_manager::get_instance()->is_userinterface_setup) {
        avatar_setup();
    }

    if(Game_manager::get_instance()->is_userinterface_setup &&
            Game_manager::get_instance()->is_avatar_setup) {
        network->component.send_ready_play();
    }
}

template<typename net_comp>
void Play<net_comp>::avatar_setup() {
    if(!Game_manager::get_instance()->is_avatar_setup) {
        network->create_avatar(player_avatar);
        Game_manager::get_instance()->is_avatar_setup = true;
    }
}

template<typename net_comp>
void Play<net_comp>::userinterface_setup() {
    if(!Game_manager::get_instance()->is_userinterface_setup) {
        switch(userinterface::progress) {
            case 0:
                {
                    score = new Score(sf::Vector2f(1280 / 2, 25.5f));
                    break;
                }
            case 1:
                {
                    deck = new Deck(sf::Vector2f(825.0f , 205.0f));
                    break;
                }
            case 2:
                {
                    chat = new Chat(sf::Vector2f(210.0f, 504.5f));
                    network->component.set_chat(chat);
                    Game_manager::get_instance()->is_userinterface_setup = true;
                    break;
                }
        }
        userinterface::progress++;
    }
}

template<typename net_comp>
Play<net_comp>::~Play() {
    if(score) delete score;
    if(deck) delete deck;
    if(chat) delete chat;
    if(play_hand) delete play_hand;
    if(placed_card) delete placed_card;
    Game_manager::reset();
    Card_manager::reset();
    userinterface::progress = 0;
}

template<typename net_comp>
void Play<net_comp>::pause() {
}

template<typename net_comp>
void Play<net_comp>::resume() {
}

template<typename net_comp>
void Play<net_comp>::turn_anim(const sf::Time& delta_time) {
    if(turn_anim_finished) return;
    if(!network->is_connected()) return;
    sf::Color color = turn_sprite.getColor();
    if(network->is_my_turn()) {
        if(color.a < 255 && !is_turn_vis) {
            color.a = static_cast<sf::Uint8>(
                std::min(255.0f, color.a + std::ceil(delta_time.asSeconds() * 400.0f)));
            if(color.a >= 255.0f) {
                is_turn_vis = true;
                turn_vis_timer = sf::Time::Zero;
            }
        }
    }
    if(is_turn_vis) {
        turn_vis_timer += delta_time;
        if(turn_vis_timer >= sf::seconds(0.5f)) {
            color.a = static_cast<sf::Uint8>(std::max(0.0f, color.a - std::floor(delta_time.asSeconds() * 200.0f)));
            if(color.a <= 0.0f) {
                is_turn_vis = false;
                turn_vis_timer = sf::Time::Zero;
                turn_anim_finished = true;
            }
        }
    }
    turn_sprite.setColor(color);
}

template<typename net_comp>
void Play<net_comp>::reset_turn_anim() {
    turn_anim_finished = false;
    is_turn_vis = false;
    turn_vis_timer = sf::Time::Zero;
    sf::Color color = turn_sprite.getColor();
    color.a = 0;
    turn_sprite.setColor(color);
}

template<typename net_comp>
void Play<net_comp>::update(const sf::Time& delta_time) {
    sf::Event event;
    while(window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                {
                    network.reset();
                    state_manager.exit();
                    break;
                }
            case sf::Event::MouseButtonPressed:
                {
                    Input_manager::button_clicked(event.mouseButton.button,
                            sf::Mouse::getPosition(window));
                    break;
                }
            case sf::Event::MouseButtonReleased:
                {
                    Input_manager::button_released(event.mouseButton.button,
                            sf::Mouse::getPosition(window));
                    break;
                }
            case sf::Event::KeyPressed:
                {
                    handle_keyboard_input(event.key.code, true);
                    break;
                }
            case sf::Event::KeyReleased:
                {
                    handle_keyboard_input(event.key.code, false);
                    break;
                }
            case sf::Event::TextEntered:
                {
                    handle_text_input(event.text.unicode);
                    break;
                }
            case sf::Event::MouseWheelScrolled:
                {
                    if(chat) {
                        chat->scroll(static_cast<int>(event.mouseWheelScroll.delta));
                    }
                    break;
                }
            default:
                break;
        }
    }
    if(network) {
        if(network->has_issue()) {
            if(!switched_menu) {
                next = State_manager::build<Network_issue<net_comp>, net_comp>(state_manager, window, &network, false);
                switched_menu = true;
                return;
            }
        }
        if(network->is_connected()) {
            network->update(delta_time);
        } else {
            next = State_manager::build<Main_menu>(state_manager, window);
            switched_menu = true;
            return;
        }
    } else {
        return;
    }
    if(!network) return;

    switch (Game_manager::get_instance()->state) {
        case Game_manager::STATE::GAME_BEGIN:
        {
            start_game(delta_time);
            break;
        }
        case Game_manager::STATE::ROUND_BEGIN:
        {
            if(network) {
                network->calc_deck_size(delta_time);
            }

            // Draw guesses
            if(!Game_manager::get_instance()->is_guess_drawn) {
                guess_hand->add_card(Card_manager::get_instance()->draw_guess());
                if(guess_hand->size() == Game_manager::get_instance()->round_number + 1) {
                    guess_hand->spawn();
                    Game_manager::get_instance()->is_guess_drawn = true;
                }
            }

            // Draw hand
            if(!Game_manager::get_instance()->is_hand_fully_drawn) {
                play_hand->add_card(network->draw_card());
                if(play_hand->size() == Game_manager::get_instance()->round_number) {
                    play_hand->spawn();
                    Game_manager::get_instance()->is_hand_fully_drawn = true;
                }
            }

             // Draw initial faceup card
            if(Game_manager::get_instance()->is_guess_drawn &&
                    Game_manager::get_instance()->is_hand_fully_drawn &&
                    !Game_manager::get_instance()->is_faceup_drawn) {
                if(network->all_cards_dealt()) {
                    network->draw_faceup();
                }
            }

            if(Game_manager::get_instance()->is_faceup_drawn &&
                    Game_manager::get_instance()->is_guess_drawn &&
                    Game_manager::get_instance()->is_hand_fully_drawn) {
                   if(network->all_cards_dealt()) {
                        switch_state(delta_time, Game_manager::STATE::PLAY_GUESS);
                   }
            }
            break;
        }
        case Game_manager::STATE::PLAY_GUESS:
        {
            set_play_order();
            turn_anim(delta_time);
            network->calc_deck_size(delta_time);

            if(network->component.player_on_turn == network->component.player_list.end()) {
                network->send_next_state(Game_manager::STATE::PLAY_CARD);
                reset_turn_anim();
                Game_manager::get_instance()->is_play_order_setup = false;
            }
            break;
        }
        case Game_manager::STATE::PLAY_CARD:
        {
            if(network) {
                Guess_card* guess = Game_manager::get_instance()->played_guess;
                if(guess && !is_guess_setup) {
                    tricks_won = 0;
                    rem_guess = new Guess_card(guess->guess_amount, guess->tex_rec);
                    is_guess_setup = true;
                }
            }
            if(rem_guess) {
                if(!rem_guess->is_spawned && is_guess_setup) {
                    rem_guess->spawn({10.0f, 740.0f}, {0.9f, 0.9f});
                }
            }

            set_play_order();
            turn_anim(delta_time);
            if(network->component.player_on_turn == network->component.player_list.end()) {
                reset_turn_anim();
                network->send_next_state(Game_manager::STATE::TRICK_END);
                Game_manager::get_instance()->is_play_order_setup = false;
            }
            break;
        }
        case Game_manager::STATE::TRICK_END:
        {
            reset_turn_anim();
            Play_card* wc = det_trick_winner_card();
            Base_player* winner = dec_winner_guess(wc);
            if(reduced_guess) {
                if(winner) {
                    reorder_to_win(winner);
                }
            }
            if(reorderd) {
                if(play_hand->size() > 0) {
                    clear_board();
                    switch_state(delta_time, Game_manager::STATE::PLAY_CARD);
                } else {
                    clear_board(true);
                    switch_state(delta_time, Game_manager::STATE::ROUND_END);
                }
            }
            break;
        }
        case Game_manager::STATE::ROUND_END:
        {
            reset_turn_anim();

            if(!finished_round) {
                calc_scores();
                Game_manager::round_reset();
                Card_manager::get_instance()->round_reset();


                for(std::vector<Avatar>::iterator itr = player_avatar.begin();
                    itr != player_avatar.end(); itr++) {
                    itr->discard_guess();
                }

                if(rem_guess) {
                    delete rem_guess;
                }
                rem_guess = nullptr;
                is_guess_setup = false;
                tricks_won = 0;

                // Shuffle Deck
                if(!Card_manager::get_instance()->shuffled) {
                    shuffle_deck();
                }

                finished_round = true;
            } else {
                if(deck->size() > 0) {
                    inc_initial_order();
                    switch_state(delta_time, Game_manager::STATE::ROUND_BEGIN);
                } else {
                    switch_state(delta_time, Game_manager::STATE::GAME_END);
                }
            }
            break;
        }
        case Game_manager::STATE::GAME_END:
        {
            reset_turn_anim();

            if(has_won()) {
                if(!switched_menu) {
                    next = State_manager::build<Won>(state_manager, window, Game_manager::get_instance()->winner_score);
                    switched_menu = true;
                    return;
                }
            } else {
                if(!switched_menu) {
                    next = State_manager::build<Lost>(state_manager, window, Game_manager::get_instance()->winner_score);
                    switched_menu = true;
                    return;
                }
            }
            break;
        }
    }

    for(auto& avatar: player_avatar) {
        avatar.update(delta_time);
    }

    if(deck) {
        deck->set_card_amount(Card_manager::get_instance()->deck_size);
    }
    if(Game_manager::get_instance()->is_userinterface_setup) {
        score->update(delta_time, window);
        deck->update(delta_time);
        chat->update(delta_time);
    }
    Base_card* played_card = nullptr;
    if(play_hand) {
        played_card = play_hand->update(window);
    }
    if(guess_hand && !played_card) {
        played_card = guess_hand->update(window);
    }

    if(played_card && !network->is_my_turn()) {
        played_card->back_to_hand();
    }
    if(played_card && network->is_my_turn()) {
        if(network->send_play(played_card, play_hand->get_cards())) {
            Guess_card* guess = dynamic_cast<Guess_card*>(played_card);
            if(guess) {
                placed_card->play(guess);
                guess_hand->discard();
                network->next_players_turn();
            }
            Play_card* play = dynamic_cast<Play_card*>(played_card);
            if(play) {
                placed_card->play(play);
                play_hand->discard(play);
                network->next_players_turn();
            }
        } else {
            played_card->back_to_hand();
        }
    }


    if(placed_card) {
        placed_card->update(window);
    }
}

template<typename net_comp>
void Play<net_comp>::draw() {
    window.clear(sf::Color::White);
    window.draw(bg_sprite);
    for(auto& avatar: player_avatar) {
        if(network) {
            if(network->component.player_on_turn != network->component.player_list.end()) {
                avatar.set_on_turn(&*network->component.player_on_turn);
            } else {
                avatar.set_on_turn(nullptr);
            }
        }
        avatar.draw(window);
    }
    if(rem_guess) {
        if(rem_guess->is_spawned && is_guess_setup) {
            rem_guess->draw(window);
        }
    }
    if(Game_manager::get_instance()->is_userinterface_setup) {
        score->draw(window);
        deck->draw(window);
        chat->draw(window);
    }
    if(placed_card) {
        placed_card->draw(window);
    }
    if(guess_hand) {
        guess_hand->draw(window);
    }
    if(play_hand) {
        play_hand->draw(window);
    }
    window.draw(turn_sprite);
    window.display();
}

template <typename net_comp>
void Play<net_comp>::inc_initial_order() {
    if(!inc_init_ord) {
        Game_manager::get_instance()->i_p_o_itr++;
        if(Game_manager::get_instance()->i_p_o_itr == Game_manager::get_instance()->initial_play_order.end()) {
            Game_manager::get_instance()->i_p_o_itr = Game_manager::get_instance()->initial_play_order.begin();
        }
        network->reorder_play_list(*Game_manager::get_instance()->i_p_o_itr);
        inc_init_ord = true;
    }
}

template<typename net_comp>
void Play<net_comp>::set_play_order() {
    cleared_board = false;
    reduced_guess = false;
    reorderd = false;
    winner_det = false;
    inc_init_ord = false;
    finished_round = false;
    if(!network) return;

    if(!Game_manager::get_instance()->is_play_order_setup) {
        Player_list::Iterator<Base_player> tmp = network->component.player_list.begin();
        network->component.player_on_turn = network->component.player_list.begin();
        if(network->component.player_on_turn != network->component.player_list.end() && tmp == network->component.player_list.end()) {
            Game_manager::get_instance()->is_last = true;
        }
        Game_manager::get_instance()->total_guesses = 0;
        Game_manager::get_instance()->is_play_order_setup = true;
    }
}

template <typename net_comp>
Play_card* Play<net_comp>::det_trick_winner_card() {
    Play_card* max = nullptr;
    if(!network) return nullptr;
    if(!winner_det) {
        for(Player_list::Iterator<Base_player> itr = network->component.player_list.begin();
            itr != network->component.player_list.end(); itr++) {
            Play_card::max(itr->played_card , max, Game_manager::get_instance()->dealing_color, Game_manager::get_instance()->trump_color);

        }
        winner_det = max == nullptr;
    }
    return max;
}

template <typename net_comp>
Base_player* Play<net_comp>::dec_winner_guess(Play_card* max) {
    if(!max) return nullptr;
    Base_player* winner;
    if(!reduced_guess) {
        for(std::vector<Avatar>::iterator itr = player_avatar.begin();
            itr != player_avatar.end(); itr++) {
            if(itr->get_card() == max) {
                itr->dec_guess();
                winner = itr->player;
            }
        }
        if(placed_card->get_card() == max) {
            dec_rem_guess();
            tricks_won++;
            winner = &network->component.player_list.get_self();
        }
        reduced_guess = true;
    }
    return winner;
}

template <typename net_comp>
void Play<net_comp>::reorder_to_win(Base_player* trick_winner) {
    if(!reorderd) {
        if(trick_winner) {
            network->reorder_play_list(trick_winner->get_unique_id());
            reorderd = true;
        }
    }
}

template <typename net_comp>
void Play<net_comp>::dec_rem_guess() {
    if(is_guess_setup && rem_guess) {
        int dec_guess = rem_guess->guess_amount - 1;
        delete rem_guess;
        if(dec_guess >= -20 && dec_guess <= 20) {
            Guess_card* tmp = get_guess_card(dec_guess);
            rem_guess = new Guess_card(tmp->guess_amount, tmp->tex_rec);
            rem_guess->spawn({10.0f, 740.0f}, {0.9f, 0.9f});
        }
    }
}

template <typename net_comp>
void Play<net_comp>::clear_board(bool round_end) {
    if(!cleared_board) {
        for(Player_list::Iterator<Base_player> itr = network->component.player_list.begin();
            itr != network->component.player_list.end(); itr++) {
            itr->played_card = nullptr;
            itr->played_guess = nullptr;
            itr->rec_cards = false;
        }
        for(std::vector<Avatar>::iterator itr = player_avatar.begin();
            itr != player_avatar.end(); itr++) {
            itr->discard();
        }
        network->send_all_cards_rec = false;
        network->send_rec = 0;
        Game_manager::get_instance()->dealing_color = nullptr;
        Game_manager::get_instance()->is_last = false;
        Game_manager::get_instance()->is_play_order_setup = false;
        if(round_end) {
            Game_manager::get_instance()->faceup_card = nullptr;
            Game_manager::get_instance()->is_faceup_drawn = false;
            guess_hand->discard();
            play_hand->discard();
        }
        placed_card->discard();
        cleared_board = true;
    }
}

template <typename net_comp>
void Play<net_comp>::calc_scores() {
    for(std::vector<Avatar>::iterator itr = player_avatar.begin();
        itr != player_avatar.end(); itr++) {
        if(itr->remaining_guess) {
            if(itr->remaining_guess->guess_amount == 0) {
                itr->player->score += 20 + 10 * itr->won_tricks;
            } else {
                itr->player->score -= 10* std::abs(itr->remaining_guess->guess_amount);
            }
        }
    }
    if(rem_guess->guess_amount == 0) {
        score->add_score(20 + 10 * tricks_won);
    } else {
        score->add_score(-10 * std::abs(rem_guess->guess_amount));
    }
}

template <typename net_comp>
bool Play<net_comp>::has_won() {
    int max = INT_MIN;
    for(std::vector<Avatar>::iterator itr = player_avatar.begin();
        itr != player_avatar.end(); itr++) {
        if(max <= itr->get_score()) {
            max = itr->get_score();
        }
    }
    if(score->get_score() >= max) {
        return true;
    }
    Game_manager::get_instance()->winner_score = max;
    return false;
}

template <typename net_comp>
void Play<net_comp>::handle_keyboard_input(sf::Keyboard::Key& key, bool status) {
    if (key == sf::Keyboard::Enter && status) {
        if (chat->get_status() == CHAT_STATUS::INACTIVE) {
            chat->activate_chat();
        }
        else if (chat->get_status() == CHAT_STATUS::ACTIVE) {
            if(network) {
                chat->send(network->component.player_list.get_self().get_name());
                network->send_msg(chat->get_msg());
            }
            chat->close_chat();
        }
        else if (chat->get_status() == CHAT_STATUS::SENT) {
            chat->close_chat();
        }

    }
    if (key == sf::Keyboard::BackSpace && status) {
        chat->rm_last_chr();
    }
    if (key == sf::Keyboard::Escape && status) {
        chat->close_chat();
    }

    /**
      * DEBUG ONLY !
      */
    if(key == sf::Keyboard::W) {
        next = State_manager::build<Won>(state_manager, window, 420);
        switched_menu = true;
    }
    /**
      * DEBUG ONLY !
      */
    if(key == sf::Keyboard::A) {
        Game_manager::get_instance()->round_number = 19;
    }

}

template <typename net_comp>
void Play<net_comp>::handle_text_input(sf::Uint32 code) {
    // 127 shift-backspace
    if (code < 127 && code > 27) {
        chat->add_txt(code);
    }
}

template class Play<Host_component>;
template class Play<Client_component>;

