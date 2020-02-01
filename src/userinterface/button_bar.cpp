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

#include "button_bar.h"

Button_bar::Button_bar(sf::Vector2f origin, sf::Vector2f size, const sf::String& txt,
        Button_type left, Button_type right, float offset) : 
        bar_label(txt, origin, 40, Label::Align::TOP)  {
    hitbox.setSize(size);
    hitbox.setPosition({origin.x - size.x / 2, origin.y - size.y / 2});
    bar_tex = Resource_manager::get_texture("../assets/button_bar.png");

    bar_sprite.setTexture(bar_tex);
    left_icon.setTexture(bar_tex);
    right_icon.setTexture(bar_tex);

    bar_frame = calc_rect(left);
    left_icon.setTextureRect(bar_frame);
    left_icon.setPosition({origin.x - offset, origin.y - bar_frame.height / 2});

    bar_frame = calc_rect(right);
    right_icon.setTextureRect(bar_frame);
    right_icon.setPosition({origin.x + offset - bar_frame.width- 10,
            origin.y - bar_frame.height / 2});

    bar_frame = sf::IntRect(0,0,400,63);
    bar_sprite.setTextureRect(bar_frame);
    bar_sprite.setPosition({origin.x - bar_frame.width / 2, origin.y - bar_frame.height / 2});

    anim_time = sf::Time::Zero;
}

Button_bar::~Button_bar() {
}

sf::IntRect Button_bar::calc_rect(Button_type b_t) {
    sf::IntRect rect;
    switch(b_t) {
        case Button_type::CREATE_ROOM:
            {
                rect = sf::IntRect(0, 195, 82, 83);
                break;
            }
        case Button_type::CONNECT:
            {
                rect = sf::IntRect(402, 0, 82, 83);
                break;
            }
        case Button_type::QUIT:
            {
                rect = sf::IntRect(402, 84, 82, 83);
                break;
            }
        case Button_type::BACK:
            {
                rect = sf::IntRect(402, 169, 82, 83);
                break;
            }
        case Button_type::NORMAL:
            {
                rect = sf::IntRect(84, 195, 89, 87);
                break;
            }
        case Button_type::INVERSE:
            {
                rect = sf::IntRect(175, 195, 89, 87);
                break;
            }
    }
    return rect;
}

void Button_bar::draw(sf::RenderTarget& target) {
    target.draw(bar_sprite);
    target.draw(left_icon);
    target.draw(right_icon);
    bar_label.draw(target);
}

void Button_bar::update(sf::Window& window) {
    // Check if Mouse overlapping and Mouse Button pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
            hitbox.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
        bar_sprite.setTextureRect(sf::IntRect(0 , 63, 400, 63));
        if(anim_clock.getElapsedTime().asSeconds() > anim_time.asSeconds() + 0.13f) {
            bar_sprite.setTextureRect(sf::IntRect(0 , 130, 400, 63));
        }
    } else {
        bar_sprite.setTextureRect(bar_frame);
        anim_clock.restart();
    }
}

#include <iostream>

bool Button_bar::is_clicked() {
    Button_info* tmp = Input_manager::get_button_info(sf::Mouse::Left);
    if(tmp == nullptr) return false;
    if (hitbox.getGlobalBounds().contains(sf::Vector2f(tmp->click_pos))) {
        if (hitbox.getGlobalBounds().contains(sf::Vector2f(tmp->release_pos))) {
            return true;
        }
    } 
    return false;
}
