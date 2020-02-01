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

#include "main_menu.h"
#include <iostream>

Main_menu::Main_menu(State_manager& s_m, sf::RenderWindow& window, bool replace) :
    Base_state(s_m, window, replace), 
    create_room_button(sf::Vector2f(1280/2, 210), sf::Vector2f(400, 65), 
            "Create Room", Button_type::NORMAL, Button_type::CREATE_ROOM, 250),
    connect_button(sf::Vector2f(1280/2, 370), sf::Vector2f(400, 65),
            "Connect", Button_type::NORMAL, Button_type::CONNECT, 250),
    quit_button(sf::Vector2f(1280/2, 530), sf::Vector2f(400, 65), 
            "Quit", Button_type::NORMAL, Button_type::QUIT, 250) {
        bg_texture = Resource_manager::get_texture("../assets/main_menu.png");
        bg_sprite.setTexture(bg_texture);

}

Main_menu::~Main_menu() {
}

void Main_menu::pause() {
}

void Main_menu::resume() {
}

void Main_menu::update(const sf::Time& delta_time) {
    sf::Event event;
    while(window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                {
                    state_manager.exit();
                }
                break;
            case sf::Event::MouseButtonPressed:
                {
                    Input_manager::button_clicked(event.mouseButton.button, 
                            sf::Mouse::getPosition(window));
                }
                break;
            case sf::Event::MouseButtonReleased:
                {
                    Input_manager::button_released(event.mouseButton.button,
                            sf::Mouse::getPosition(window));
                }
                break;
            default:
                break;
        }
    }
    create_room_button.update(window);
    if(create_room_button.is_clicked()) {
        next = State_manager::build<Room_creation>(state_manager, window, false);
    }
    connect_button.update(window);
    if(connect_button.is_clicked()) {
        next = State_manager::build<Connect>(state_manager, window, false);
    }
    quit_button.update(window);
    if(quit_button.is_clicked()) {
        state_manager.exit();
    }
}

void Main_menu::draw() {
    window.clear(sf::Color::White);
    window.draw(bg_sprite);
    create_room_button.draw(window);
    connect_button.draw(window);
    quit_button.draw(window);
    window.display();
}
