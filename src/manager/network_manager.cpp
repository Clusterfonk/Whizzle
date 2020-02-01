/**
 * Derivedhis file is part of Whizzle.
 * Copyright (c) 2019 Derivedill Vogel
 *
 *  Whizzle is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Whizzle is distributed in the hope that it will be useful,
 *  but WIDerivedHOUDerived ANY WARRANDerivedY; without even the implied warranty of
 *  MERCHANDerivedABILIDerivedY or FIDerivedNESS FOR A PARDerivedICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Whizzle.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "network_manager.h"

namespace {
    std::vector<sf::Vector2f> avatar_places = {
        sf::Vector2f(76.0f, 100.0f),
        sf::Vector2f(1205.0f, 100.0f),
        sf::Vector2f(76.0f, 283.0f),
        sf::Vector2f(1205.0f, 283.0f),
        sf::Vector2f(1205.0f, 470.0f)
    };

    std::vector<sf::Vector2f>::iterator itr = avatar_places.begin();

    sf::Vector2f get_next_avatar_place() {
        sf::Vector2f ret = (*itr);
        itr++;
        return ret;
    }
}

template<typename Derived>
Network_manager<Derived>::Network_manager() : component() {
}

template<typename Derived>
Network_manager<Derived>::~Network_manager() {
    itr = avatar_places.begin();
}

template<typename Derived>
void Network_manager<Derived>::connect() {
}

template<typename Derived>
void Network_manager<Derived>::disconnect() {
    component.disconnect();
}

template<typename Derived>
void Network_manager<Derived>::update(const sf::Time& delta_time) {
    component.update(delta_time);
}

template<typename Derived>
void Network_manager<Derived>::create_avatar(std::vector<Avatar>& avatar_list) {
    component.enemy_mutex.lock(); //!Lock
    for(Player_list::Iterator<Remote_player> itr = component.player_list.begin();
         itr != component.player_list.end(); itr++) {
        avatar_list.emplace_back(Avatar(get_next_avatar_place(), &(*itr)));
    }
    component.enemy_mutex.unlock(); //!Unlock
}

template<typename Derived>
bool Network_manager<Derived>::is_connected() {
    return component.is_connected();
}

template<typename Derived>
bool Network_manager<Derived>::has_issue() {
    return component.has_issue();
}

template<typename Derived>
size_t Network_manager<Derived>::get_connection_count() {
    return component.get_connection_count();
}

template <>
Play_card* Network_manager<Host_component>::draw_card() {
    return Card_manager::draw_card();
}

template <>
Play_card* Network_manager<Client_component>::draw_card() {
    if(send_rec <= static_cast<int>(Game_manager::get_instance()->round_number)
            && component.hand_buffer.size() <= 0) {
        component.send_card_request();
    }
    Play_card* drawn = component.receive_card_draw();
    if(drawn) {
        send_rec++;
    }
    return drawn;
}

template <>
Play_card* Network_manager<Host_component>::draw_faceup() {
    Card_manager::get_instance()->card_mutex.lock(); // !Lock
    Game_manager::get_instance()->faceup_card = draw_card();
    if(Game_manager::get_instance()->faceup_card) {
        if(Game_manager::get_instance()->faceup_card->color != card_color::neutral) {
            Game_manager::get_instance()->trump_color = &Game_manager::get_instance()->faceup_card->color;
        }
    } else {
        Game_manager::get_instance()->faceup_card = nullptr;
        Game_manager::get_instance()->trump_color = nullptr;
    }
    Game_manager::get_instance()->is_faceup_drawn = true;
    component.broadcast_faceup(Game_manager::get_instance()->faceup_card);
    Card_manager::get_instance()->card_mutex.lock(); // !Unlock
    return Game_manager::get_instance()->faceup_card;
}

template <>
Play_card* Network_manager<Client_component>::draw_faceup() {
    if(component.is_card_received) {
        component.is_card_received = false;
        return Game_manager::get_instance()->faceup_card;
    }
    return nullptr;
}


template <>
bool Network_manager<Host_component>::send_play(Base_card* played, std::vector<Play_card*>& hand) {
    Guess_card* guess = dynamic_cast<Guess_card*>(played);
    if(guess) {
        if(Game_manager::get_instance()->check_play(guess)) {
            Card_manager::get_instance()->last_played_card = played;
            Card_manager::get_instance()->can_play_last = true;
            component.broadcast_guess(guess);
            return true;
        }
    }
    Play_card* play = dynamic_cast<Play_card*>(played);
    if(play) {
        if(Game_manager::get_instance()->check_play(play, hand)) {
            component.player_list.get_self().played_card = play;
            Card_manager::get_instance()->last_played_card = played;
            Card_manager::get_instance()->can_play_last = true;
            component.broadcast_play(play);
            return true;
        }
    }
    return false;
}

template <>
bool Network_manager<Client_component>::send_play(Base_card* played, std::vector<Play_card*>& hand) {
    Guess_card* guess = dynamic_cast<Guess_card*>(played);
    if(guess) {
        if(Game_manager::get_instance()->check_play(guess)) {
            Card_manager::get_instance()->last_played_card = played;
            Card_manager::get_instance()->can_play_last = true;
            component.send_card_play(guess);
            return true;
        }
    }
    Play_card* play = dynamic_cast<Play_card*>(played);
    if(play) {
        if(Game_manager::get_instance()->check_play(play, hand)) {
            component.player_list.get_self().played_card = play;
            Card_manager::get_instance()->last_played_card = played;
            Card_manager::get_instance()->can_play_last = true;
            component.send_card_play(play);
            return true;
        }
    }
    return false;
}

template <>
void Network_manager<Host_component>::send_next_state(Game_manager::STATE state) {
    component.send_next_state(state);
}

template <>
void Network_manager<Client_component>::send_next_state(Game_manager::STATE state) {
}

template<typename Derived>
void Network_manager<Derived>::next_players_turn() {
    if(component.player_on_turn != component.player_list.end()) {
        component.player_on_turn++;
        Player_list::Iterator<Base_player> tmp = component.player_on_turn;
        tmp++;
        if(component.player_on_turn != component.player_list.end() && tmp == component.player_list.end()) {
            Game_manager::get_instance()->is_last = true;
        }
    }
}

template<>
void Network_manager<Client_component>::calc_deck_size(const sf::Time& delta_time) {
}

template<>
void Network_manager<Host_component>::calc_deck_size(const sf::Time& delta_time) {
    Card_manager::get_instance()->card_mutex.lock(); // !Lock
    deck_size_update += delta_time;
    if(deck_size_update >= sf::seconds(1.0f)) {
        Card_manager* inst = Card_manager::get_instance();
        int size = static_cast<int>(inst->crt_card - inst->deck.begin());
        if(size != inst->deck_size) {
            inst->deck_size = 59 - size;
            component.broadcast_deck_size(inst->deck_size);
        }
        deck_size_update = sf::Time::Zero;
    }
    Card_manager::get_instance()->card_mutex.unlock(); // !Unlock
}

template<typename Derived>
bool Network_manager<Derived>::is_my_turn() {
    Base_player* p_on_t = nullptr;
    try {
//        if(!this) return false;
        if(component.player_on_turn != component.player_list.end()) {
            p_on_t = static_cast<Local_player*>(&*(component.player_on_turn));
        }
    } catch(std::exception e) {

    }
    if(!p_on_t) return false;
    Base_player* self = &(component.player_list.get_self());
    if(!self) return false;
    return self == p_on_t;
}

template<typename Derived>
void Network_manager<Derived>::reorder_play_list(boost::uuids::uuid winner_id) {
    component.reorder(winner_id);
}

template <>
bool Network_manager<Host_component>::all_cards_dealt() {
    bool ret = true;
    for(Player_list::Iterator<Remote_player> itr = component.player_list.begin();
        itr != component.player_list.end(); itr++) {
        ret &= itr->rec_cards;
    }
    return ret;
}

template <>
bool Network_manager<Client_component>::all_cards_dealt() {
    if(!send_all_cards_rec) {
        component.send_rec_all_cards();
        send_all_cards_rec = true;
    }
    return send_all_cards_rec;
}

template <>
void Network_manager<Host_component>::send_msg(std::string msg) {
    component.broadcast_msg(msg, component.player_list.get_self().get_unique_id());
}

template <>
void Network_manager<Client_component>::send_msg(std::string msg) {
    component.send_msg(msg);
}

template class Network_manager<Host_component>;
template class Network_manager<Client_component>;
