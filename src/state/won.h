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

#ifndef WON_HEADER
#define WON_HEADER

#include <SFML/Graphics.hpp>

#include "base_state.h"
#include "manager/input_manager.h"
#include "manager/resource_manager.h"
#include "userinterface/button_bar.h"

class Won : public Base_state {
    public:
        Won(State_manager& s_m, sf::RenderWindow& window, int winner_score, bool replace);
        ~Won();

        void pause();
        void resume();

        void update(const sf::Time& delta_time);
        void draw();

    private:
        Button_bar play_button;
        Button_bar quit_button;

        Label winner_score;

        sf::Sprite bg_sprite;
        sf::Texture bg_texture;

};
#endif
