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

#include "avatar.h"
#include <iostream>

Avatar::Avatar(sf::Vector2f origin, Base_player* p) :
        player(p),
        avatar_name(p->get_name(), {origin.x + 8.0f, origin.y - 60.0f}, 20, Label::Align::TOP),
        avatar_score("0", {origin.x + 8.0f, origin.y +  70.0f}, 14, Label::Align::TOP) {
    avatar_tex = Resource_manager::get_texture("../assets/avatar.png");
    avatar_sprite.setTexture(avatar_tex);
    avatar_sprite.setPosition(origin.x - avatar_sprite.getGlobalBounds().width / 2,
                              origin.y - avatar_sprite.getGlobalBounds().height / 2);
    sf::Color color = avatar_sprite.getColor();
    color.a = 0;
    avatar_sprite.setColor(color);
    avatar_name.set_color(255, 255, 255, 0);
    if(origin.x < 1280.0f / 2.0f) {
        play_card_pos = origin + sf::Vector2f(170.0f, 0.0f);
        guess_card_pos = origin + sf::Vector2f(-50.0f, 46.0f);
    } else {
        play_card_pos = origin - sf::Vector2f(170.0f, 0.0f);
        guess_card_pos = origin + sf::Vector2f(50.0f, 46.0f);
    }
    player->score = 0;
}

Avatar::~Avatar() {
}

void Avatar::set_on_turn(Base_player* cmp_player) {
    if(player) {
        is_on_turn = player == cmp_player;
    } else {
        is_on_turn = false;
    }
}

void Avatar::dec_guess() {
    if(remaining_guess) {
        int dec_guess = remaining_guess->guess_amount - 1;
        if(dec_guess >= -20 && dec_guess <= 20) {
            won_tricks += 1;
            Guess_card* tmp = get_guess_card(dec_guess);
            if(tmp) {
                delete remaining_guess;
                remaining_guess = nullptr;
                remaining_guess = new Guess_card(tmp->guess_amount, tmp->tex_rec);
                if(!remaining_guess->is_spawned) {
                    remaining_guess->spawn(guess_card_pos, {0.5f, 0.5f});
                }
                remaining_guess->moveable = false;
            }
        }
    }
}

void Avatar::discard() {
    if(player) {
        if(player->played_card) {
            player->played_card->reset();
            player->played_card = nullptr;
        }
    }
}

void Avatar::draw(sf::RenderTarget& target) {
    avatar_sprite.setTexture(avatar_tex);
    target.draw(avatar_sprite);
    avatar_name.draw(target);
    avatar_score.draw(target);

    if(remaining_guess) {
        if(remaining_guess->is_spawned) {
            remaining_guess->draw(target);
        }
    }

    if(player->played_card) {
        if(player->played_card->is_spawned) {
            player->played_card->draw(target);
        }
    }
}

void Avatar::update(const sf::Time& delta_time) {
    sf::Color color = avatar_sprite.getColor();
    if(color.a < 255) {
        color.a = static_cast<sf::Uint8>(
                    std::min(255.0f, color.a + std::ceil(delta_time.asSeconds() + fading_velocity)));
    }
    avatar_sprite.setColor(color);

    if(is_on_turn) {
        avatar_name.set_color(0, 255, 0, color.a);
    } else {
        avatar_name.set_color(255, 255, 255, color.a);
    }

    if(player->played_card) {
        if(!player->played_card->is_spawned) {
            player->played_card->spawn(play_card_pos, {0.7f, 0.7f});
        }
    }

    if(!remaining_guess) {
        if(player) {
            if(player->played_guess) {
                remaining_guess = new Guess_card(player->played_guess->guess_amount, player->played_guess->tex_rec);
                if(!remaining_guess->is_spawned) {
                    remaining_guess->spawn(guess_card_pos, {0.46f, 0.46f});
                }
                remaining_guess->moveable = false;
            }
        }
    }

    if(player) {
        avatar_score.set_string(std::to_string(player->score));
    }
}

Play_card* Avatar::get_card() {
    if(player) {
        if(player->played_card) {
            return player->played_card;
        }
    }
    return nullptr;
}

void Avatar::discard_guess() {
    player->played_guess = nullptr;
    delete remaining_guess;
    remaining_guess = nullptr;
    won_tricks = 0;
}

int Avatar::get_score() {
    return player->score;
}
