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

#include "room_creation.h"


Room_creation::Room_creation(State_manager& s_m, sf::RenderWindow& window, bool replace) :
    Base_state(s_m, window, replace),
    network(std::make_unique<Network_manager<Host_component>>()),
    play_button(sf::Vector2f(1280/2, 436), sf::Vector2f(400, 65), 
            "Play", Button_type::NORMAL, Button_type::CREATE_ROOM, 215.0f),
    back_button(sf::Vector2f(1280/2, 562), sf::Vector2f(400, 65),
            "Back", Button_type::BACK, Button_type::INVERSE, 215.0f),
    wait_bar(sf::Vector2f(1280/2, 320)),
    id_cards(sf::Vector2f(1280/2, 180.5f), network->component.get_ip_addr()),
    bubble(sf::Vector2f(1280, 720)),
    current_connections(std::to_string(network->get_connection_count()),
                        sf::Vector2f(476, 438), 30, Label::Align::TOP) {
        bg_texture = Resource_manager::get_texture("../assets/room_creation_base.png");
        bg_sprite.setTexture(bg_texture);
}

#include <iostream>
Room_creation::~Room_creation() {
    network.reset();
}

void Room_creation::pause() {
}

void Room_creation::resume() {
}

void Room_creation::update(const sf::Time& delta_time) {
    sf::Event event;
    while(window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                {
                    network.reset();
                    state_manager.exit();
                    break;
                }
            case sf::Event::MouseButtonPressed:
                {
                    Input_manager::button_clicked(event.mouseButton.button, 
                            sf::Mouse::getPosition(window));
                    break;
                }
            case sf::Event::MouseButtonReleased:
                {
                    Input_manager::button_released(event.mouseButton.button,
                            sf::Mouse::getPosition(window));
                    break;
                }
            default:
                break;
        }
    }
    if(network) {
        network->component.update(delta_time);
    }
    play_button.update(window);
    if(play_button.is_clicked()) {
        if(network && !starting_session) {
            if(network->component.start_play()) {
                starting_session = true;
                next = State_manager::build<Play<Host_component>, Host_component>(state_manager, window, &network);
            }
        }
        Input_manager::clear_mouse_inputs();
    }
    back_button.update(window);
    if(back_button.is_clicked()) {
        state_manager.last_state();
    }

    wait_bar.update(delta_time);
    id_cards.update(delta_time);
    bubble.update(delta_time);

    if(network) {
        current_connections.set_string(std::to_string(network->get_connection_count()));
        if(network->get_connection_count() >= 3 && network->get_connection_count() < 6) {
            wait_bar.set_type(Waiting_bar::type::READY);
        } else if (network->get_connection_count() == 6) {
            wait_bar.set_type(Waiting_bar::type::FULL);
        } else {
            wait_bar.set_type(Waiting_bar::type::WAITING);
        }
    }
}

void Room_creation::draw() { 
    window.clear(sf::Color::White);

    window.draw(bg_sprite);
    play_button.draw(window);
    back_button.draw(window);
    wait_bar.draw(window);
    id_cards.draw(window);
    bubble.draw(window);
    current_connections.draw(window);

    window.display();
}
