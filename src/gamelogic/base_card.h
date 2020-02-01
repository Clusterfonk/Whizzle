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

#ifndef BASE_CARD_HEADER
#define BASE_CARD_HEADER

#include <SFML/Graphics.hpp>

#include "manager/resource_manager.h"
#include "manager/input_manager.h"

class Base_card {
    public:
         Base_card(sf::IntRect tex_pos, sf::Texture& tex);
         virtual ~Base_card() = 0;

         void spawn(sf::Vector2f pos);
         void spawn(sf::Vector2f pos, sf::Vector2f scale);

         void draw(sf::RenderTarget& target);
         Base_card* update(sf::Window& window);

         void placed(sf::Vector2f pos);
         bool moveable = true;

         static Base_card* selected_card;

         bool is_spawned = false;
         void back_to_hand();
    protected:
         sf::Sprite card_sprite;

         sf::Vector2f start_position;
         sf::Vector2f ref_position;

         bool is_moveing = false;
    private:
         static sf::Texture card_texture;
         void move(sf::Vector2f target_pos);

};
#endif
