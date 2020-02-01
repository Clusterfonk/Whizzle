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

#include "waiting_bar.h"

Waiting_bar::Waiting_bar(sf::Vector2f origin) :
    bar_label("", {origin.x, origin.y}, 24, Label::Align::TOP) {
    bar_tex = Resource_manager::get_texture("../assets/creation_bar.png");
    bar_sprite.setPosition(origin.x - 306, origin.y - 20);
    bar_label.set_color(0, 0, 0, 255);
    set_type(type::WAITING);
}

Waiting_bar::~Waiting_bar() {
}

void Waiting_bar::set_type(type bar_type) {
    if (crt_type == bar_type) {
        return;
    }
    sf::IntRect frame_loc;
    std::string display_text;
    switch(bar_type) {
        case type::FULL:
            {
                crt_type = type::FULL;
                frame_loc = sf::IntRect(385, 177, 612, 40);
                display_text = "Full";
                break;
            }
        case type::WAITING:
            {
                crt_type = type::WAITING;
                frame_loc = sf::IntRect(385, 217, 612, 40);
                display_text = "Waiting for Players";
                break;
            }
        case type::READY:
            {
                crt_type = type::READY;
                frame_loc = sf::IntRect(385, 257, 612, 40);
                display_text = "Ready";
                break;
            }
        default:
            {
                return;
            }
    }
    bar_label.set_string(display_text);
    bar_label.resize();
    bar_sprite.setTextureRect(frame_loc);
    bar_sprite.setTexture(bar_tex);
}

void Waiting_bar::draw(sf::RenderTarget& target) {
    target.draw(bar_sprite);
    bar_label.draw(target);
}

// TODO: check connection count or manager directly which type to choose 
// probably static to manager
// TODO: set text waiting for x more player
// x players are ready
// or just full
// press play maybe animation for waiting bar IntRect gets smaller might look like draining the loading bar
void Waiting_bar::update(const sf::Time& delta_time) {
}
