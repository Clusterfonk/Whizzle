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

#ifndef ROOM_CREATION_HEADER 
#define ROOM_CREATION_HEADER 

#include "base_state.h"
#include "main_menu.h"
#include "play.h"
#include "manager/resource_manager.h"
#include "manager/network_manager.h"
#include "network/host_component.h"
#include "userinterface/button_bar.h"
#include "userinterface/waiting_bar.h"
#include "userinterface/room_id_cards.h"
#include "userinterface/notification_bubble.h"
#include "userinterface/label.h"

class Room_creation : public Base_state {
    public:
        Room_creation(State_manager& s_m, sf::RenderWindow& window, bool replace);
        ~Room_creation();

        void pause();
        void resume();

        void update(const sf::Time& delta_time);
        void draw();

    private:
        std::unique_ptr<Network_manager<Host_component>> network;

        sf::Sprite bg_sprite;
        sf::Texture bg_texture;

        Button_bar play_button;
        Button_bar back_button;

        Waiting_bar wait_bar;
        Room_id_cards id_cards;
        Notification_bubble bubble;
        Label current_connections;

        bool starting_session = false;

};

#endif


