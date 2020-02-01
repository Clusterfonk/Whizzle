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

#ifndef SCORE_HEADER
#define SCORE_HEADER

#include <string>
#include <SFML/Graphics.hpp>

#include "label.h"
#include "manager/resource_manager.h"
#include "manager/input_manager.h"

class Score {
public:
    Score(sf::Vector2f origin);
    Score(sf::Vector2f origin, int init_score);
    ~Score();

    void draw(sf::RenderTarget& target);
    void update(const sf::Time& delta_time, sf::Window& window);

    int get_score();
    void add_score(int add_score);
private:
    sf::Texture tex;
    sf::Sprite sprite;

    Label score_display;

    bool active = false;
    float velocity = 40.0f;
    float limit_origin_y;
    float limit_subtracted_y = - 45.0f;
    sf::Time inactive_timer = sf::seconds(4.0f);
};

#endif
