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

#include "deck.h"

Deck::Deck(sf::Vector2f origin, int amount) {
    assert(amount <= 60);
    tex = Resource_manager::get_texture("../assets/cards.png");
    sf::IntRect tex_rec = sf::IntRect({1648, 316}, {103, 158});


    if(amount > 0 && amount < 10) {
        card_amount = 1;
    } else {
        card_amount = static_cast<int>(amount / 10);
    }

    pos = {
        origin,
        sf::Vector2f(origin.x - 1.4f, origin.y - 2.2f),
        sf::Vector2f(origin.x - 3.2f, origin.y - 4.7f),
        sf::Vector2f(origin.x - 5.0f, origin.y - 7.2f),
        sf::Vector2f(origin.x - 6.8f, origin.y - 9.7f),
        sf::Vector2f(origin.x - 8.6f, origin.y - 12.2f),
    };

    pos_itr = pos.begin();

    for(int i = 0; i < card_amount; i++) {
        sf::Sprite sprite;
        sprite.setTextureRect(tex_rec);
        sprite.setTexture(tex);
        sprite.rotate(11.0f);
        sf::Vector2f sprite_pos = get_next_pos();
        sprite.setPosition(sprite_pos.x - sprite.getGlobalBounds().width / 2,
                           (sprite_pos.y - sprite.getGlobalBounds().height / 2));
        card_sprites.emplace_back(sprite);
    }
}

Deck::~Deck() {
    card_sprites.clear();
    pos.clear();
}

void Deck::draw(sf::RenderTarget& target) {
    // ensures i + 1 sprite is ontop of 0 to i
    for(auto& sprite: card_sprites) {
        target.draw(sprite);
    }
}

size_t Deck::size() {
    return static_cast<size_t>(card_amount);
}

void Deck::update(const sf::Time& delta_time) {
}

void Deck::set_card_amount(int new_amount) {
    if(new_amount > 0 && new_amount < 10) {
        new_amount = 1;
    } else {
        new_amount = static_cast<int>(new_amount / 10);
    }
    if(card_amount == new_amount) return;
    if(card_amount > new_amount) {
        for(int i = 0; i <= card_amount - new_amount; i++) {
            if(card_sprites.size() > 0) {
                card_sprites.pop_back();
                pos_itr--;
            }
        }
        card_amount = new_amount;
    } else if(card_amount < new_amount) {
        for(int i = 0; i <= new_amount - card_amount; i++) {
            sf::Sprite sprite;
            sprite.setTexture(tex);
            sf::Vector2f sprite_pos = get_next_pos();
            sprite.setPosition(sprite_pos.x - sprite.getGlobalBounds().width / 2,
                               (sprite_pos.y - sprite.getGlobalBounds().height / 2));
            card_sprites.emplace_back(sprite);
        }
        card_amount = new_amount;
    }
}

sf::Vector2f Deck::get_next_pos() {
    sf::Vector2f ret = (*pos_itr);
    pos_itr++;
    return ret;
}
