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

#include "state_manager.h"

State_manager::State_manager() : resume(false), running(false) {
}

void State_manager::run(std::unique_ptr<Base_state> initial_state) {
	running = true;
	state_stack.push(std::move(initial_state));
}

void State_manager::next_state() {
	if(resume) { 
		if(!state_stack.empty()) {
			state_stack.pop();
		}
		if(!state_stack.empty()) {
			state_stack.top()->resume();
		}
		resume = false;
	}
	if(!state_stack.empty()) {
		std::unique_ptr<Base_state> tmp = state_stack.top()->get_next();
		if (tmp != nullptr) {
			if (tmp->is_clearing()) {
				while (state_stack.size() > 1) {
					state_stack.pop();
                }
			}
			if(tmp->is_replacing()) { // remove state and push new onto
				state_stack.pop();
            } else { // pause state and push new onto
				state_stack.top()->pause();
            }
			state_stack.push(std::move(tmp));
		}
	}
}

void State_manager::last_state() {
    Input_manager::clear_mouse_inputs();
	resume = true;
}

void State_manager::update(sf::Time delta_time) {
	state_stack.top()->update(delta_time);
}

void State_manager::draw() {
	state_stack.top()->draw();
}

bool State_manager::is_running() {
    return running;
}

void State_manager::exit() {
    running = false;
    while (state_stack.empty()) {
        state_stack.pop();
    }
}
