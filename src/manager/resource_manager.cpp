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

#include "resource_manager.h"

Resource_manager* Resource_manager::instance = nullptr;

Resource_manager::Resource_manager() {
    instance = this;
}

Resource_manager* Resource_manager::get_instance() {
    if(!instance) {
        instance = new Resource_manager();
    }
    return instance;
}

sf::Texture& Resource_manager::get_texture(std::string const& file_path) {
    auto& map = Resource_manager::get_instance()->textures;
    auto pair = map.find(file_path);

    if(pair != map.end()) {
        return pair->second;
    } else {
        auto& tex = map[file_path];
        tex.loadFromFile(file_path);
        return tex;
    }
}

sf::SoundBuffer& Resource_manager::get_sound(std::string const& file_path) {
    auto& map = Resource_manager::get_instance()->sounds;
    auto pair = map.find(file_path);

    if(pair != map.end()) {
        return pair->second;
    } else {
        auto& sound_buffer = map[file_path];
        sound_buffer.loadFromFile(file_path);
        return sound_buffer;
    }
}


sf::Font& Resource_manager::get_font(std::string const& file_path) {
    auto& map = Resource_manager::get_instance()->fonts;
    auto pair = map.find(file_path);

    if(pair != map.end()) {
        return pair->second;
    } else {
        auto& font = map[file_path];
        font.loadFromFile(file_path);
        return font;
    }
}
