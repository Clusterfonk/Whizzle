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

#include "input_manager.h"

Input_manager* Input_manager::instance = nullptr; 

Input_manager::Input_manager() {
    instance = this;

    left_info = new Button_info();
    right_info = new Button_info();
    clear_mouse_inputs();
}

Input_manager* Input_manager::get_instance() {
    if(Input_manager::instance == nullptr) {
        Input_manager::instance = new Input_manager();
    }
    return Input_manager::instance;
}

void Input_manager::button_clicked(sf::Mouse::Button button, sf::Vector2i pos) {
    if(instance == nullptr) return;
    switch (button) {
        case sf::Mouse::Button::Left: 
            {
                if(instance != nullptr && !instance->left_pressed) {
                    instance->left_info->click_pos = pos;
                    instance->left_info->release_pos = {-1, -1};
                    instance->left_info->pressed = true;
                    instance->left_pressed = true;
                }
                break;
            }
        case sf::Mouse::Button::Right:
            {
                if(instance != nullptr && !instance->right_pressed) {
                    instance->right_info->click_pos = pos;
                    instance->right_info->release_pos = {-1, -1};
                    instance->right_info->pressed = false;
                    instance->right_pressed = true;
                }
                break;
            }
        default:
            break;
    }
}

void Input_manager::button_released(sf::Mouse::Button button, sf::Vector2i pos) {
    if(instance == nullptr) return;
    switch(button) {
        case sf::Mouse::Button::Left: 
            {
                instance->left_info->release_pos = pos;
                instance->left_pressed = false;
                instance->left_info->pressed = false;
                break;
            }
        case sf::Mouse::Button::Right:
            {
                instance->right_info->release_pos = pos;
                instance->right_pressed = false;
                instance->right_info->pressed = false;
                break;
            }
        default:
            break;
    }
}

/** 
 * Sets unreachable mouse values
 */
void Input_manager::clear_mouse_inputs() {
    if(instance == nullptr) return;
    instance->left_pressed = false;
    instance->right_pressed = false;
    instance->left_info->click_pos = {-1, -1}; 
    instance->left_info->release_pos = {-1, -1};
    instance->right_info->click_pos = {-1, -1};
    instance->right_info->release_pos = {-1, -1};
}

Button_info* Input_manager::get_button_info(sf::Mouse::Button button) {
    if (button == sf::Mouse::Button::Left) {
        auto tmp = Input_manager::get_instance()->left_info;
        return tmp;
    } else if(button == sf::Mouse::Button::Right) {
        auto tmp = Input_manager::get_instance()->right_info;
        return tmp;
    }
    return nullptr;
}
