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

#ifndef LABEL_HEADER
#define LABEL_HEADER 

#include <SFML/Graphics.hpp>

#include "manager/resource_manager.h"

class Label {
    public:

        enum class Align {
            TOP, BOT
        };

        Label();
        Label(const sf::String& text, sf::Vector2f pos, unsigned int size, Align align);
        void set_string(const std::string& text);
        void set_position(sf::Vector2f pos);
        void set_size(unsigned int size);
        void draw(sf::RenderTarget &target);
        void set_color(int r, int g, int b, int a);
        void set_alignment(Align a);
        void resize();
        sf::Text txt;

    private:
        Align alignment;
        sf::Vector2f origin;
};
#endif
