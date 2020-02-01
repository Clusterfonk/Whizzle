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

#include "player_list.h"

Player_list::Player_list() {
}

Player_list::~Player_list() {
}

void Player_list::emplace_back(Remote_player* new_ele) {
    Node** indirect = &head;

    while(*indirect) {
        indirect = &((*indirect)->next);
    }
    *indirect = new Node();
    (*indirect)->data = new_ele;
    (*indirect)->type = Type::REMOTE;
    if((*indirect)->data->get_unique_id().is_nil()) host = (*indirect);
    trail_remote = (*indirect);
    connection_count++;
}

void Player_list::emplace_back(Local_player* new_ele) {
    Node** indirect = &head;

    while(*indirect) {
        indirect = &((*indirect)->next);
    }
    (*indirect) = new Node();
    (*indirect)->data = new_ele;
    (*indirect)->type = Type::LOCAL;
    if(self == nullptr) self = (*indirect);
    trail_local = (*indirect);
    connection_count++;
}

Remote_player& Player_list::get_host() {
    assert(host != nullptr);
    return *dynamic_cast<Remote_player*>(host->data);
}

Local_player& Player_list::get_self() {
    assert(self != nullptr);
    return *dynamic_cast<Local_player*>(self->data);
}

void Player_list::clear() {
    Player_list::Node::clear(head);
    head = nullptr;
    trail_local = nullptr;
    trail_remote = nullptr;
    host = nullptr;
    self = nullptr;
    connection_count = 0;
}

// Recursively delete all Nodes following n
void Player_list::Node::clear(Node* n) {
    if(!n) return;
    if(n->next) clear(n->next);
    delete n->data;
    delete n;
}


bool Player_list::contains(boost::uuids::uuid id) {
    Node** indirect = &head;
    while(*indirect) {
        if((*indirect)->data->get_unique_id() == id) return true;
        indirect = &((*indirect)->next);
    }
    return false;
}

void Player_list::swap_to_end(boost::uuids::uuid id) {
    Node** prev = &head;
    Node** indirect = prev;

    while(*indirect) {
        if((*indirect)->data->get_unique_id() == id) break;
        prev = &(*indirect);
        indirect = &((*indirect)->next);
    }
    Node** last = indirect;
    while((*last)->next) {
        last = &((*last)->next);
    }
    if(*indirect != *last) {
        Node* tmp = (*indirect);
        (*prev)->next = tmp->next;
        (*last)->next = tmp;
        if(head == tmp) head = tmp->next;
        tmp->next = nullptr;
        if(tmp->type == Type::REMOTE) {
            trail_remote = (*indirect);
        } else if(tmp->type == Type::LOCAL) {
            trail_local = (*indirect);
        }
    }
}

void Player_list::swap_to_end(unsigned int index) {
    Node** prev = &head;
    Node** indirect = prev;
    unsigned int current_index = 0;

    while(*indirect) {
        if(current_index == index) break;
        prev = &(*indirect);
        indirect = &((*indirect)->next);
        current_index++;
    }
    if(current_index != index) return;
    Node** last = &(*indirect);
    while((*last)->next) {
        last = &((*last)->next);
    }
    if(*indirect != *last) {
        Node* tmp = *indirect;
        (*prev)->next = tmp->next;
        (*last)->next = tmp;
        if(head == tmp) head = tmp->next;
        tmp->next = nullptr;
        if(tmp->type == Type::REMOTE) {
            trail_remote = (*indirect);
        } else if(tmp->type == Type::LOCAL) {
            trail_local = (*indirect);
        }
    }
}

size_t Player_list::size() {
    return connection_count;
}

template <typename T>
void Player_list::erase(Player_list::Iterator<T> pos) {
    Node** prev = &head;
    Node** indirect = prev;

    while(*indirect && (*indirect) != pos.itr) {
        prev = &(*indirect);
        indirect = &((*indirect)->next);
    }

    Node* tmp = *indirect;
    Node* next = (*indirect)->next;
    if(tmp == head) {
        head = next;
    } else {
        (*prev)->next = next;
    }
    if(tmp->type == Type::REMOTE && tmp == trail_remote) {
        trail_remote = get_trailing(tmp->type);
    } else if(tmp->type == Type::LOCAL && tmp == trail_local) {
        trail_local = get_trailing(tmp->type);
    }

    delete tmp->data;
    delete tmp;

    connection_count--;
}

Player_list::Node* Player_list::get_trailing(Type type) {
    Node** indirect = &head;
    while(*indirect && (*indirect)->type != type) {
        indirect = &((*indirect)->next);
    }
    return *indirect;
}

template <>
Remote_player& Player_list::back() {
    return dynamic_cast<Remote_player&>(*trail_remote->data);
}

template <>
Local_player& Player_list::back() {
    return dynamic_cast<Local_player&>(*trail_local->data);
}

Player_list::Iterator<Base_player> Player_list::begin() {
    return Player_list::Iterator<Base_player>(head);
}

Player_list::Iterator<Base_player> Player_list::end() {
    return Player_list::Iterator<Base_player>(nullptr);
}

template <>
Player_list::Iterator<Base_player>::operator Player_list::Iterator<Remote_player>() {
    while(itr && itr->type != Type::REMOTE) {
        itr = itr->next;
    }
    return Player_list::Iterator<Remote_player>(itr);
}

template <>
Player_list::Iterator<Base_player>::operator Player_list::Iterator<Local_player>() {
    while(itr && itr->type != Type::LOCAL) {
        itr = itr->next;
    }
    return Player_list::Iterator<Local_player>(itr);
}

template <>
Player_list::Iterator<Remote_player> Player_list:: Iterator<Remote_player>::operator++ () {
    if(itr) {
        do {
            itr = itr->next;
        } while(itr && itr->type != Type::REMOTE);
    }
    return *this;
}

template <>
Player_list::Iterator<Local_player> Player_list:: Iterator<Local_player>::operator++ () {
    if(itr) {
        do {
            itr = itr->next;
        } while(itr && itr->type != Type::LOCAL);
    }
    return *this;
}

template <>
Player_list::Iterator<Base_player> Player_list:: Iterator<Base_player>::operator++ () {
    if(itr) {
        itr = itr->next;
    }
    return *this;
}

template <>
Player_list::Iterator<Remote_player> Player_list::Iterator<Remote_player>::operator++ (int) {
    Iterator<Remote_player> iterator = *this;
    ++*this;
    return iterator;
}

template <>
Player_list::Iterator<Local_player> Player_list::Iterator<Local_player>::operator++ (int) {
    Iterator<Local_player> iterator = *this;
    ++*this;
    return iterator;
}

template <>
Player_list::Iterator<Base_player> Player_list::Iterator<Base_player>::operator++ (int) {
    Iterator<Base_player> iterator = *this;
    ++*this;
    return iterator;
}

template<>
Remote_player& Player_list::Iterator<Remote_player>::operator*() {
    return *dynamic_cast<Remote_player*>(itr->data);
}

template<>
Local_player& Player_list::Iterator<Local_player>::operator*() {
    return *dynamic_cast<Local_player*>(itr->data);
}

template<>
Base_player& Player_list::Iterator<Base_player>::operator*() {
    return *itr->data;
}

template <>
Remote_player* Player_list::Iterator<Remote_player>::operator->() {
    return dynamic_cast<Remote_player*>(itr->data);
}

template <>
Local_player* Player_list::Iterator<Local_player>::operator->() {
    return dynamic_cast<Local_player*>(itr->data);
}

template <>
Base_player* Player_list::Iterator<Base_player>::operator->() {
    return itr->data;
}

void Player_list::shuffle(boost::random_device& gen) {
    std::vector<unsigned int> new_order(connection_count);
    std::iota(new_order.begin(), new_order.end(), 0);
    std::shuffle(new_order.begin(), new_order.end(), gen);
    for(std::vector<unsigned int>::iterator itr = ++new_order.begin(); itr != new_order.end(); itr++) {
        swap_to_end(*itr);
    }
}

template class Player_list::Iterator<Remote_player>;
template class Player_list::Iterator<Local_player>;
template void Player_list::erase<Remote_player>(Player_list::Iterator<Remote_player> pos);
template void Player_list::erase<Local_player>(Player_list::Iterator<Local_player> pos);
