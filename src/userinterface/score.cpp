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

#include "score.h"
#include <iostream>

Score::Score(sf::Vector2f origin) {
    score_display = Label("0", {origin.x + 5.0f, origin.y - 5.0f}, 45, Label::Align::TOP);
    tex = Resource_manager::get_texture("../assets/score.png");
    sprite.setTexture(tex);
    sprite.setPosition(origin.x - sprite.getGlobalBounds().width / 2,
                        origin.y - sprite.getGlobalBounds().height / 2);
    limit_origin_y = sprite.getPosition().y;
    score_display.set_color(0, 0, 0, 255);
}

Score::Score(sf::Vector2f origin, int init_score) {
    score_display = Label(std::to_string(init_score), origin, 16, Label::Align::TOP);
    tex = Resource_manager::get_texture("../assets/score.png");
    sprite.setTexture(tex);
    limit_origin_y = origin.y - sprite.getGlobalBounds().height / 2;
    sprite.setPosition(origin.x - sprite.getGlobalBounds().width / 2,
                       limit_origin_y);
    score_display.set_color(0, 0, 0, 255);
}

Score::~Score() {
}

void Score::draw(sf::RenderTarget& target) {
    target.draw(sprite);
    score_display.draw(target);
}

void Score::update(const sf::Time& delta_time, sf::Window& window) {
    if (sprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
        active = true;
    }

    float posx = sprite.getPosition().x;
    float posy = sprite.getPosition().y;
    score_display.set_position(sf::Vector2f(posx + sprite.getGlobalBounds().width / 2  + 5.0f,
                                            posy + sprite.getGlobalBounds().height / 2 - 5.0f));
    if(active) {
        if(posy < limit_origin_y) {
            sprite.setPosition(posx, posy + delta_time.asSeconds() * velocity);
        } else {
            // Sprite is fully extended
            sprite.setPosition(posx, limit_origin_y);
            inactive_timer = sf::Time::Zero;
            active = false;
        }
    } else {
        inactive_timer += delta_time;
        if(inactive_timer >= sf::seconds(4.0f)) {
            if(posy > limit_subtracted_y) {
                sprite.setPosition(posx, posy - delta_time.asSeconds() * velocity);
            } else {
                // Sprite is fully subtracted
                sprite.setPosition(posx, limit_subtracted_y);
                inactive_timer = sf::seconds(4.0f);
            }
        }
    }
}

int Score::get_score() {
    try {
        std::string score = score_display.txt.getString();
        return std::stoi(score);
    } catch (std::invalid_argument const &e) {
        // TOOD: logging
        std::cout << e.what() << std::endl;
        return 0;
    } catch (std::out_of_range const& e) {
        // TOOD: logging
        std::cout << e.what() << std::endl;
        return 0;
    }
}

void Score::add_score(int add_score) {
    int old_score = get_score();
    int new_score = old_score + add_score;
    if(old_score == new_score) return;
    try {
        score_display.txt.setString(std::to_string(new_score));
    } catch (std::bad_alloc const& e) {
        // TOOD: error correction
        return;
    }

    if (old_score > new_score) {
        // TODO: negative animation start
    } else if (old_score < new_score) {
        // TODO: posiitve animation start
    }

}
