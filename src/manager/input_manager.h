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

#ifndef INPUT_MANAGER_HEADER
#define INPUT_MANAGER_HEADER

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <assert.h>

struct Button_info {
    sf::Vector2i click_pos;
    sf::Vector2i release_pos;
    bool pressed;

    Button_info() {};
    Button_info(sf::Vector2i c, sf::Vector2i r) : click_pos(c), release_pos(r) {
    }
};


class Input_manager : sf::NonCopyable {
    public:
        static Input_manager* get_instance();

        static void clear_mouse_inputs();
        static void button_clicked(sf::Mouse::Button button, sf::Vector2i position);
        static void button_released(sf::Mouse::Button button, sf::Vector2i position);
        static Button_info* get_button_info(sf::Mouse::Button button);

    private:
        Input_manager();
        static Input_manager* instance;

        bool left_pressed;
        sf::Vector2i left_click_pos;
        sf::Vector2i left_release_pos;
        Button_info* left_info;

        bool right_pressed;
        sf::Vector2i right_click_pos;
        sf::Vector2i right_release_pos;
        Button_info* right_info;
};
#endif
