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

#ifndef CORE_HEADER
#define CORE_HEADER

#include "manager/input_manager.h"
#include "manager/state_manager.h"
#include "state/main_menu.h"

/**
 * Provides all core functionalities
 */
class Core {
	public:
		Core();
		~Core();

		void run();

	private:
        State_manager state_manager;
		sf::RenderWindow window;

        sf::Clock clock;
        const sf::Time delta_time = sf::seconds(1.0f / 60.0f); 
        sf::Time last_update_time;
};

#endif
