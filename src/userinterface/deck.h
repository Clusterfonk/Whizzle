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

#ifndef DECK_HEADER
#define DECK_HEADER

#include <assert.h>
#include <SFML/Graphics.hpp>

#include "manager/resource_manager.h"

class Deck {
public:
    Deck(sf::Vector2f origin, int card_amount = 60);
    ~Deck();

    void draw(sf::RenderTarget& target);
    void update(const sf::Time& delta_time);

    void set_card_amount(int new_amount);

    size_t size();

private:
    sf::Texture tex;
    std::vector<sf::Sprite> card_sprites;

    /**
      Determines how many cards are displayed in the Deck
     */
    int card_amount;

    std::vector<sf::Vector2f> pos;
    std::vector<sf::Vector2f>::iterator pos_itr;
    sf::Vector2f get_next_pos();
};

#endif
