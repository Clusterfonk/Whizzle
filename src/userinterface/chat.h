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

#ifndef CHAT_HEADER
#define CHAT_HEADER

#include <SFML/Graphics.hpp>

#include "manager/resource_manager.h"
#include "scroll_text_box.h"
#include "chat_entry.h"
#include "player/base_player.h"

enum CHAT_STATUS {
    ACTIVE,
    SENT,
    INACTIVE
};

/**
 * Graphical Entity that displays and handles the functionality
 * to communicate with other players in the Session.
 *
 * Consists of two main elements:
 *
 * Scroll_text_box: displays up to 9 messages.
 *
 * Chat_entry: Userinput that can be send over the network to other players in the session.
 */
class Chat {
public:
    Chat(sf::Vector2f origin);
    ~Chat();

    void draw(sf::RenderTarget& target);
    void update(const sf::Time& delta_time);

    bool is_active();
    void set_position(sf::Vector2f pos);
    void set_player(Base_player* player);

    void activate_chat();
    void send(const std::string& own_name);
    void close_chat();

    void add_player_msg(const std::string& msg, const std::string& player_name);
    void add_msg(const std::string& msg);

    void add_txt(sf::Uint32 char_code);
    void rm_last_chr();

    CHAT_STATUS get_status();
    void set_status(CHAT_STATUS status);
    std::string get_msg();

    void scroll(int direction);

private:
    sf::Texture tex;
    sf::Sprite sprite;

    Scroll_text_box content;
    Chat_entry text_entry;

    CHAT_STATUS status;
};

#endif
