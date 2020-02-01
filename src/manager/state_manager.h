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

#ifndef STATE_MANAGER_HEADER
#define STATE_MANAGER_HEADER

#include <memory>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <stack>

#include "state/base_state.h"
#include "manager/input_manager.h"
#include "manager/network_manager.h"

class Base_state; // Forward declaration (circular dependency)

/**
 * Class that is a state factory and manages these
 */
class State_manager {
    public:
        State_manager();

        void run(std::unique_ptr<Base_state> initial_state);

        void next_state();
        void last_state();

        void update(sf::Time delta_time);
        void draw();

        bool is_running();
        void exit();

        template <typename T>
        static std::unique_ptr<T> build(State_manager& s_m, 
                sf::RenderWindow& window,
                bool replace = true) {
            // Clear all mouse Inputs upon creation
            Input_manager::clear_mouse_inputs();
            return std::unique_ptr<T>(std::make_unique<T>(s_m, window, replace));
        }

        template <typename T, typename D>
        static std::unique_ptr<T> build(State_manager& s_m,
                sf::RenderWindow& window,
                std::unique_ptr<Network_manager<D>>* net,
                bool replace = true) {
            // Clear all mouse Inputs upon creation
            Input_manager::clear_mouse_inputs();
            return std::unique_ptr<T>(std::make_unique<T>(s_m, window, net, replace));
        }

        template <typename T>
        static std::unique_ptr<T> build(State_manager& s_m,
                sf::RenderWindow& window,
                int winner_score,
                bool replace = true) {
            // Clear all mouse Inputs upon creation
            Input_manager::clear_mouse_inputs();
            return std::unique_ptr<T>(std::make_unique<T>(s_m, window, winner_score, replace));
        }

    private:
        std::stack<std::unique_ptr<Base_state>> state_stack;

        bool resume;
        bool running;
};
#endif
