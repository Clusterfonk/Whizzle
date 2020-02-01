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

#ifndef BASE_STATE_HEADER
#define BASE_STATE_HEADER

#include <SFML/System/Time.hpp>
#include <memory>
#include <stack>

#include "manager/state_manager.h"

class State_manager; // Forward declaration avoid circular dependencies

class Base_state : sf::NonCopyable {
    public:
        Base_state(State_manager& s_m, sf::RenderWindow& window, bool replace = true);
        virtual ~Base_state() = default;

        virtual void update(const sf::Time& delta_time) = 0;
        virtual void draw() = 0;

        virtual void pause() = 0;
        virtual void resume() = 0;

        std::unique_ptr<Base_state> get_next();

        bool is_replacing();
        bool is_clearing();

    protected:
        State_manager& state_manager;
        sf::RenderWindow& window;

        std::unique_ptr<Base_state> next;

        bool marked_replace;
        bool marked_clear;
};
#endif
