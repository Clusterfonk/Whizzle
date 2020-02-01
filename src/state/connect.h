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

#ifndef CONNECT_HEADER
#define CONNECT_HEADER

#include "base_state.h"
#include "main_menu.h"
#include "play.h"
#include "manager/resource_manager.h"
#include "manager/input_manager.h"
#include "manager/network_manager.h"
#include "network/client_component.h"
#include "userinterface/button_bar.h"
#include "userinterface/waiting_bar.h"
#include "userinterface/room_id_cards.h"
#include "userinterface/label.h"
#include "userinterface/text_entity.h"


class Connect : public Base_state {
    public:
        Connect(State_manager& s_m, sf::RenderWindow& window, bool replace);
        ~Connect();

        void pause();
        void resume();

        void update(const sf::Time& delta_time);
        void draw();
    private:
        std::unique_ptr<Network_manager<Client_component>> network;

        sf::Sprite bg_sprite;
        sf::Texture bg_texture;

        Button_bar con_button;
        Button_bar back_button;

        Label request_input;
        Text_entity input_entity;

        float input_error_timer = 0.0f;


    private:
        void input_error();
        void confirm_input();
};
#endif
