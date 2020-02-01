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

#ifndef NOTIFICATION_BUBBLE_HEADER
#define NOTIFICATION_BUBBLE_HEADER

#include <cmath>
#include <cstdint>
#include <queue>
#include <SFML/Graphics.hpp>

#include "label.h"
#include "manager/resource_manager.h"

class Notification_bubble {
    public:
        Notification_bubble(sf::Vector2f origin);
        ~Notification_bubble();

        void draw(sf::RenderTarget& target);
        void update(const sf::Time& delta_time);
        static void notify(std::string content);
        static void disc(std::string content);

    private:
        sf::Color sprite_color;
        float crt_alpha;
        sf::Texture bubble_tex;
        sf::Sprite bubble_sprite;

        Label not_txt;
        sf::SoundBuffer not_buffer;
        sf::Sound not_sound;

        static std::queue<std::string> new_players;
        static std::queue<std::string> disc_players;

        sf::Clock anim_clock;
        sf::Time anim_time;

        void (Notification_bubble::*crtn_anim_func)(void) = nullptr;
        void fade_animation();
        void fade_out_animation();

    private:
        static sf::Mutex notif_mutex;
        static sf::Mutex disc_mutex;
};
#endif
