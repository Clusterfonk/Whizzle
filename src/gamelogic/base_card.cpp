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

#include "base_card.h"

namespace {
    sf::IntRect play_hitbox = sf::IntRect(180, 30, 910, 340);
}

Base_card* Base_card::selected_card = nullptr;

Base_card::Base_card(sf::IntRect tex_pos, sf::Texture& tex) {
    card_sprite.setTextureRect(tex_pos);
    card_sprite.setTexture(tex);
}

Base_card::~Base_card() {
}

void Base_card::spawn(sf::Vector2f pos) {
    start_position = pos;
    card_sprite.setPosition({pos.x - card_sprite.getGlobalBounds().width / 2.0f,
                            pos.y - card_sprite.getGlobalBounds().height / 2.0f});
    is_spawned = true;
}

void Base_card::spawn(sf::Vector2f pos, sf::Vector2f scale) {
    card_sprite.setScale(scale);
    spawn(pos);
}

void Base_card::draw(sf::RenderTarget& target) {
    if(!is_spawned) return;
    target.draw(card_sprite);
}

Base_card* Base_card::update(sf::Window& window) {
    if (!is_spawned ||
            (Base_card::selected_card && Base_card::selected_card != this)) {
        return nullptr;
    }
    if(moveable) {
        Button_info* tmp = Input_manager::get_button_info(sf::Mouse::Left);
        if(!is_moveing &&
                card_sprite.getGlobalBounds().contains(sf::Vector2f(tmp->click_pos)) &&
                tmp->pressed) {
            is_moveing = true;
            ref_position = sf::Vector2f(tmp->click_pos);
            Base_card::selected_card = this;
        } else if(!tmp->pressed) {
            is_moveing = false;
            if (play_hitbox.contains(tmp->release_pos) && Base_card::selected_card) {
                Base_card::selected_card = nullptr;
                Input_manager::clear_mouse_inputs();
                return this;
            } else {
                back_to_hand();
                Input_manager::clear_mouse_inputs();
            }
        }
        if(is_moveing) {
            sf::Vector2f tmp = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            move(sf::Vector2f(tmp.x - ref_position.x, tmp.y - ref_position.y));
            ref_position = tmp;
        }
    }
    return nullptr;
}

void Base_card::move(sf::Vector2f target_pos) {
    card_sprite.setPosition(card_sprite.getPosition().x + target_pos.x,
                            card_sprite.getPosition().y + target_pos.y);
}

void Base_card::placed(sf::Vector2f pos) {
    card_sprite.setPosition({pos.x - card_sprite.getGlobalBounds().width / 2.0f,
                            pos.y - card_sprite.getGlobalBounds().height / 2.0f});
}

void Base_card::back_to_hand() {
    card_sprite.setPosition(start_position.x - card_sprite.getGlobalBounds().width / 2,
                start_position.y - card_sprite.getGlobalBounds().height / 2);
    Base_card::selected_card = nullptr;
}
