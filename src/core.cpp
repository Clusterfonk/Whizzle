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
#include <iostream>

Core::Core() {
}

Core::~Core() {
}

void Core::run() {
	displayInit();
    Network_Manager n_m;

	sf::Clock clock;
	sf::Time delta_time;

	while (window->isOpen()) {
		updateInput();

        delta_time = clock.restart();
        // TODO: Update with dt

		window->clear();
		// TODO: Rendering
		window->display();
	}
}

void Core::displayInit() {
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "Whizzle", sf::Style::Default);
	window->setFramerateLimit(60);
}

void Core::updateInput() {
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::KeyPressed) {
		}
		if (event.type == sf::Event::KeyReleased) {
		}
		if (event.key.code == sf::Keyboard::Escape || 
				event.type == sf::Event::Closed) {
			// TODO: Handleing keyboard events
			// TODO: Handleing Mouse events
			// TODO: Update Game Objects in the scene
			window->close();
		}
	}
}
