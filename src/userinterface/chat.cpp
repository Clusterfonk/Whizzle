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

#include "chat.h"

Chat::Chat(sf::Vector2f origin) : content({origin.x, origin.y + 110.0f}, {600.0f, 180.0f}),
    text_entry({origin.x - 165.0f, origin.y + 90.0f}, 20, 26) {
    tex = Resource_manager::get_texture("../assets/chat.png");
    sprite.setTexture(tex);
    sprite.setPosition(origin.x - sprite.getGlobalBounds().width / 2,
                       origin.y - sprite.getGlobalBounds().height /2);
    close_chat();
}

Chat::~Chat() {
}

void Chat::draw(sf::RenderTarget& target) {
    target.draw(sprite);

    content.draw(target);
    text_entry.draw(target);

}

void Chat::update(const sf::Time& delta_time) {
    content.update(delta_time);
    text_entry.update(delta_time);
}

void Chat::activate_chat() {
    status = CHAT_STATUS::ACTIVE;
    text_entry.clear();
    text_entry.set_active(true);
}

void Chat::set_position(sf::Vector2f pos) {
    text_entry.set_position({pos.x - 200.0f, pos.y - 50.0f});
}

void Chat::send(const std::string& own_name) {
    std::string msg = text_entry.get_string();
    if(msg.size() > 0) {
        add_player_msg(msg, own_name);
    }
    status = CHAT_STATUS::INACTIVE;
}

void Chat::close_chat() {
    text_entry.clear();
    text_entry.set_active(false);
    status = CHAT_STATUS::INACTIVE;
}

void Chat::add_player_msg(const std::string &msg, const std::string& player_name) {
    content.add_string("[" + player_name + "]: " + msg);
    text_entry.set_active(false);
    status = CHAT_STATUS::SENT;
}

void Chat::add_msg(const std::string &msg) {
    content.add_string(msg);
}

void Chat::add_txt(sf::Uint32 char_code) {
    if(status == CHAT_STATUS::ACTIVE) {
        text_entry.add_char(static_cast<char>(char_code));
    }
}

void Chat::rm_last_chr() {
    text_entry.rm_char();
}

CHAT_STATUS Chat::get_status() {
    return status;
}

std::string Chat::get_msg() {
    return text_entry.get_string();
}

/**
 * Scroll the Scroll_Text_Box in the given direction.
 * Input_arg comes from MouseWheelScrolled_delta which is -1 or +1
 * direction = 1 => UP
 * direction = -1 => DOWN
 */
void Chat::scroll(int direction) {
    content.scroll(direction);
}
