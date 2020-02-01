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

#ifndef AVATAR_HEADER
#define AVATAR_HEADER

#include <cmath>
#include <SFML/Graphics.hpp>

#include "label.h"
#include "manager/card_manager.h"
#include "manager/resource_manager.h"
#include "player/base_player.h"

class Avatar {
public:
    Avatar(sf::Vector2f origin, Base_player* player);
    ~Avatar();

    void set_on_turn(Base_player* cmp_player);
    void dec_guess();

    void draw(sf::RenderTarget& target);
    void update(const sf::Time& delta_time);

    void discard();
    void discard_guess();


    Play_card* get_card();
    Base_player* player;

    int won_tricks = 0;
    int get_score();
    Guess_card* remaining_guess = nullptr;

private:
    sf::Texture avatar_tex;
    sf::Sprite avatar_sprite;
    Label avatar_name;
    Label avatar_score;

    sf::Vector2f play_card_pos;

    sf::Vector2f guess_card_pos;

    float fading_velocity = 1.0f;
    bool is_on_turn = false;
};

#endif
