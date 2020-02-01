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

#ifndef PLACED_CARD_HEADER
#define PLACED_CARD_HEADER

#include "SFML/Graphics.hpp"

#include "gamelogic/base_card.h"
#include "gamelogic/play_card.h"
#include "manager/card_manager.h"
#include "utility/convo.h"

class Placed_card {
public:
    Placed_card(sf::Vector2f origin);

    void play(Base_card* played_card);

    void discard();

    void draw(sf::RenderTarget& target);
    void update(sf::Window& window);

    Play_card* get_card();

private:
    sf::Vector2f card_pos;
    Base_card* card;
};

#endif
