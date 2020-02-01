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

#ifndef RESOURCE_MANAGER_HEADER
#define RESOURCE_MANAGER_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Resource_manager : sf::NonCopyable {
    public:
        Resource_manager();
        static Resource_manager* get_instance();

        static sf::Texture& get_texture(std::string const& file_path);
        static sf::SoundBuffer& get_sound(std::string const& file_path);
        static sf::Font& get_font(std::string const& file_path);

    private:
        std::map<std::string, sf::Texture> textures;
        std::map<std::string, sf::SoundBuffer> sounds;
        std::map<std::string, sf::Font> fonts;

        static Resource_manager* instance;
};

#endif
