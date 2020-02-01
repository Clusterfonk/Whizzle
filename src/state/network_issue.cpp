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

#include "network_issue.h"

template <typename net_comp>
Network_issue<net_comp>::Network_issue(State_manager& s_m, sf::RenderWindow& window,
                    std::unique_ptr<Network_manager<net_comp>>* net, bool replace) :
                    Base_state (s_m, window, replace),
                    resume_button(sf::Vector2f(1280.0f / 2.0f, 410.0f), sf::Vector2f(400.0f, 65.0f),
                                  "Retry", Button_type::NORMAL, Button_type::CONNECT, 215.0f),
                    quit_button(sf::Vector2f(1280.0f / 2.0f, 535.0f), sf::Vector2f(400.0f, 65.0f),
                                  "Quit", Button_type::QUIT, Button_type::QUIT, 215.0f) {
    if(net) {
        network = std::move(*net);
    }
    bg_texture = Resource_manager::get_texture("../assets/network_issue.png");
    bg_sprite.setTexture(bg_texture);
    paused_texture = Resource_manager::get_texture("../assets/play_base.png");
    paused_sprite.setTexture(paused_texture);
}

template <typename net_comp>
Network_issue<net_comp>::~Network_issue() {
    window.clear(sf::Color::White);
    window.draw(bg_sprite);
}

template <typename net_comp>
void Network_issue<net_comp>::pause() {
}

template <typename net_comp>
void Network_issue<net_comp>::resume() {
}

template <typename net_comp>
void Network_issue<net_comp>::update(const sf::Time& delta_time) {
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
        network->update(delta_time);
    }
    resume_button.update(window);
    if(resume_button.is_clicked()) {
    }
    quit_button.update(window);
    if(quit_button.is_clicked()) {
        network.reset();
        next = State_manager::build<Main_menu>(state_manager, window, true);
    }
}

template <typename net_comp>
void Network_issue<net_comp>::draw() {
    window.clear(sf::Color::White);
    window.draw(paused_sprite);
    window.draw(bg_sprite);
    resume_button.draw(window);
    quit_button.draw(window);
    window.display();
}


template class Network_issue<Host_component>;
template class Network_issue<Client_component>;
