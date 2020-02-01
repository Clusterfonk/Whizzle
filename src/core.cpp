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

#include "core.h"

Core::Core() {
    window.setVerticalSyncEnabled(true);
    window.create(sf::VideoMode(1280, 720), "Whizzle", sf::Style::Close); 
    state_manager.run(State_manager::build<Main_menu>(state_manager, window, true));
}

Core::~Core() {
}

void Core::run() {
    last_update_time = sf::Time::Zero;

    while(state_manager.is_running()) {
        state_manager.next_state();

        while(last_update_time > delta_time) {
            last_update_time -= delta_time;
            state_manager.update(delta_time);
        }
        state_manager.draw();
        last_update_time += clock.restart();
    }
}
