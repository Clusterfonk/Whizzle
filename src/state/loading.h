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

#ifndef LOADING_HEADER
#define LOADING_HEADER

#include <SFML/Graphics.hpp>

#include "base_state.h"
#include "main_menu.h"


class Loading : public Base_state {
    public:
        Loading(State_manager& s_m, sf::RenderWindow& window, bool replace);
        ~Loading();

        void pause();
        void resume();

        void update(const sf::Time& delta_time);
        void draw();
    private:
        std::unique_ptr<Network_manager<Client_component>> network;
        sf::Sprite bg_sprite;
        sf::Texture bg_texture;
};
#endif
