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

#ifndef PLAYER_LIST_HEADER
#define PLAYER_LIST_HEADER

#include <assert.h>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/irange.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include "player/local_player.h"
#include "player/remote_player.h"

class Player_list {
public:
    Player_list();
    ~Player_list();

    void emplace_back(Remote_player* new_ele);
    void emplace_back(Local_player* new_ele);
    Remote_player& get_host();
    Local_player& get_self();
    void clear();
    void shuffle(boost::random_device& gen);
    void swap_to_end(boost::uuids::uuid id);
    void swap_to_end(unsigned int index);
    bool contains(boost::uuids::uuid id);

    size_t size();

    template <typename T>
    T& back();

    // Player_list Iterator
public:
    template <typename T>
    class Iterator;

    Iterator<Base_player> begin();
    Iterator<Base_player> end();

    template <typename T>
    void erase(Iterator<T> pos);

    // Node struct
private:
    enum class Type {
        NONE,
        REMOTE,
        LOCAL
    };

    struct Node {
    public:
        friend class Player_list;

        Node* next = nullptr;
        Base_player* data;
    private:
        Type type = Type::NONE;

        bool operator!=(const Node& rhs) {
            return (type != rhs.type);
        }

        bool operator==(const Node& rhs) {
            return (type == rhs.type);
        }

        static void clear(Node* n);
    };

private :
    Node* head = nullptr;

    Node* host = nullptr; // Host is the first added remote_player
    Node* self = nullptr; // self is the first added local_player

    Node* trail_remote = nullptr; // last remote_player
    Node* trail_local = nullptr; // last local_player

    Node* get_trailing(Type type);

    size_t connection_count = 0;

    // Iterator class
public:
    template <typename T>
    class Iterator {
        friend class Player_list;
    public:
        Iterator<T>(Node* tmp) noexcept : itr(tmp) {}
        Iterator<T>(const Iterator<T>& citr) noexcept : itr(citr.itr) {}
        // Prefix
        Iterator<T> operator++();
        // Postfix
        Iterator<T> operator++(int);

        // Cast to Iterator<Remote_player>
        operator Iterator<Remote_player>();
        // Cast to Iterator<Local_player>
        operator Iterator<Local_player>();

        bool operator!=(const Iterator<T>& rhs) { return itr != rhs.itr;}
        bool operator==(const Iterator<T>& rhs) { return itr == rhs.itr;}

        T& operator*();
        T* operator->();

        Node* itr;
    };
};
#endif
