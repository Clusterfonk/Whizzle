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

#include "connect.h"

Connect::Connect(State_manager& s_m, sf::RenderWindow& window, bool replace) :
    Base_state(s_m, window, replace),
    network(std::make_unique<Network_manager<Client_component>>()),
    con_button(sf::Vector2f(1280/2, 436), sf::Vector2f(400, 65), 
            "Confirm", Button_type::NORMAL, Button_type::CREATE_ROOM, 215.0f),
    back_button(sf::Vector2f(1280/2, 562), sf::Vector2f(400, 65),
            "Back", Button_type::BACK, Button_type::INVERSE, 215.0f),
    request_input("Enter a Room ID", sf::Vector2f(1280/2, 160.0f), 40, Label::Align::TOP),
    input_entity(sf::Vector2f(1280/2, 236.0f), 40, true, 12) {
        bg_texture = Resource_manager::get_texture("../assets/connect_base.png");
        bg_sprite.setTexture(bg_texture);

        request_input.set_color(0, 0, 0, 255);
        input_entity.set_color(0, 0, 0, 255);
}

Connect::~Connect() {
    network.reset();
}

void Connect::pause() {
}

void Connect::resume() {
}

void Connect::update(const sf::Time& delta_time) {
    sf::Event event;
    while(window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                {
                    network.reset();
                    state_manager.exit();
                }
                break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Key::Backspace) {
                    if(!network->is_connected()) {
                        input_entity.rm_char();
                    }
                }
                else if(event.key.code == sf::Keyboard::Key::Enter) {
                    if(!network->is_connected()) {
                        confirm_input();
                    }
                }
                break;
            case sf::Event::KeyReleased:
                break;
            case sf::Event::TextEntered:
                {
                    if (event.text.unicode < 127 && event.text.unicode > 27){
                        if(!network->is_connected()) {
                            if(!input_entity.add_char(static_cast<char>(event.text.unicode))) {
                                input_error();
                            }
                        }
                    }
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
        network->update(delta_time);

        if(!network->is_connected()) {
            request_input.set_string("Enter a Room ID");
            input_entity.alpha_channel = 255;
        }

        if(network->component.is_session_started) {
            network->component.is_session_started = false; // prevents flooding
            next = State_manager::build<Play<Client_component>, Client_component>(state_manager, window, &network, true);
        }
    }

    con_button.update(window);
    if(con_button.is_clicked()) {
        Input_manager::clear_mouse_inputs();
        if(!network->is_connected()) {
            confirm_input();
        }
    }
    back_button.update(window);
    if(back_button.is_clicked()) {
        network->disconnect();
        state_manager.last_state();
    }

    input_entity.update(delta_time);

    input_error_timer += delta_time.asSeconds();
    if (input_error_timer >= 0.5f) {
        input_entity.set_color(0, 0, 0, input_entity.alpha_channel);
        input_error_timer = 0.0f;
    } 
}

void Connect::draw() {
    window.clear(sf::Color::White);
    window.draw(bg_sprite);
    con_button.draw(window);
    back_button.draw(window);
    request_input.draw(window);
    input_entity.draw(window);

    window.display();
}

/** Checks userinput and either connects or throws an error visually
 */
void Connect::confirm_input() {
    std::string tmp = input_entity.get_text();
    if (!network->component.is_connected() && tmp.size() == 6) {
        if (network->component.connect(tmp)) {
            input_entity.alpha_channel = 0;
            input_entity.clear();
            request_input.set_string("      Connected\n\nWaiting for others");
            return;
        }
    }
    input_error();
}

void Connect::input_error() {
    input_entity.set_color(255,0,0,255);
    input_error_timer = 0.0f;
}
