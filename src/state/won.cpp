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

#include "won.h"

Won::Won(State_manager& s_m, sf::RenderWindow& window, int winner_score, bool replace) :
    Base_state(s_m, window, replace),
    play_button(sf::Vector2f(1280/2, 436), sf::Vector2f(400, 65),
            "Play", Button_type::NORMAL, Button_type::CREATE_ROOM, 215.0f),
    quit_button(sf::Vector2f(1280/2, 580), sf::Vector2f(400, 65),
            "Quit", Button_type::NORMAL, Button_type::QUIT, 215),
    winner_score(std::to_string(winner_score), sf::Vector2f(1280.0f / 2.0f, 240.0f), 52, Label::Align::TOP) {
        bg_texture = Resource_manager::get_texture("../assets/you_win.png");
        bg_sprite.setTexture(bg_texture);
}

Won::~Won() {
}

void Won::pause() {
}

void Won::resume() {
}

void Won::update(const sf::Time& delta_time) {
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
    play_button.update(window);
    if(play_button.is_clicked()) {
//        next = State_manager::build<Play(s_m, window, null, true)
    }
    quit_button.update(window);
    if(quit_button.is_clicked()) {
        state_manager.exit();
    }
}

void Won::draw() {
    window.clear(sf::Color::White);
    window.draw(bg_sprite);
    winner_score.draw(window);
    play_button.draw(window);
    quit_button.draw(window);
    window.display();
}
