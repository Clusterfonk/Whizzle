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

#include "notification_bubble.h"

std::queue<std::string> Notification_bubble::new_players;
std::queue<std::string> Notification_bubble::disc_players;
sf::Mutex Notification_bubble::notif_mutex;
sf::Mutex Notification_bubble::disc_mutex;

Notification_bubble::Notification_bubble(sf::Vector2f origin) : 
        not_txt("", {origin.x - 200, origin.y - 240}, 24, Label::Align::TOP) {
    bubble_tex = Resource_manager::get_texture("../assets/creation_bar.png");
    bubble_sprite.setTexture(bubble_tex);
    bubble_sprite.setTextureRect(sf::IntRect(0, 0, 385, 295));
    bubble_sprite.setPosition(origin.x - 385, origin.y - 320);
    // Setup alpha channel
    crt_alpha = 0.0f;
    sprite_color = sf::Color(255, 255, 255, 0);
    bubble_sprite.setColor(sprite_color);
    not_txt.set_color(25, 25, 25, 255);

    // Setup sound
    not_buffer = Resource_manager::get_sound("../assets/not_ping.wav");
    not_sound.setBuffer(not_buffer);
    not_sound.setVolume(40.0f);

    anim_time = sf::Time::Zero;
}

Notification_bubble::~Notification_bubble() {
    not_sound.stop();
}

void Notification_bubble::draw(sf::RenderTarget& target) {
    target.draw(bubble_sprite);
    not_txt.draw(target);
}

void Notification_bubble::update(const sf::Time& delta_time) {
    // Check if new notification have to be done
    notif_mutex.lock(); // !Lock
    if (new_players.size() > 0 && crtn_anim_func == nullptr) {
        std::string tmp_player = new_players.front();
        not_sound.play();
        not_txt.set_string(std::string("New Player\nconnected:\n\n" + new_players.front()));
        not_txt.resize();
        new_players.pop();
        crtn_anim_func = &Notification_bubble::fade_animation;
    }
    notif_mutex.unlock(); // !Unlock
    disc_mutex.lock(); // !Lock
    if (disc_players.size() > 0 && crtn_anim_func == nullptr) {
        std::string tmp_player = disc_players.front();
        not_sound.play();
        not_txt.set_string(std::string("      Player\ndisconnected:\n\n" + disc_players.front()));
        not_txt.resize();
        disc_players.pop();
        crtn_anim_func = &Notification_bubble::fade_animation;
    }
    disc_mutex.unlock(); // !Unlock
    // Ptr to crt animation
    if(crtn_anim_func != nullptr) {
        (this->*this->crtn_anim_func)();
        sprite_color.a = static_cast<uint8_t>(crt_alpha);
        bubble_sprite.setColor(sprite_color);
        not_txt.set_color(0,0,0,crt_alpha);
    } 
}

/**
 * Fades in the notification bubble until the opacity is at 255
 */
void Notification_bubble::fade_animation() {
    if(crt_alpha >= 250) {
        if(anim_clock.getElapsedTime().asSeconds() > anim_time.asSeconds() + 2.4f) {
            crtn_anim_func = &Notification_bubble::fade_out_animation;
            anim_clock.restart();
            return;
        }
    }
    crt_alpha = std::min(250 * anim_clock.getElapsedTime().asSeconds() / 1.0f, 250.0f);
}


/**
 * Fades out the notification bubble until the opacity is at 0
 */
void Notification_bubble::fade_out_animation() {
    if(crt_alpha <= 0) {
        crtn_anim_func = nullptr;
        not_sound.stop();
        return;
    }
    crt_alpha = std::max(250 + (-250 * anim_clock.getElapsedTime().asSeconds() / 0.6f), 0.0f);
}

/**
 * Queue up a new connection notification to the room host
 */
void Notification_bubble::notify(std::string name) { 
    notif_mutex.lock(); // !Lock
    new_players.push(name);
    notif_mutex.unlock(); // !Unlock
}

/**
 * Queue up a disconnect notification to the room host
 */
void Notification_bubble::disc(std::string name) {
    disc_mutex.lock(); // !Lock
    disc_players.push(name);
    disc_mutex.unlock(); // !Unlock
}
