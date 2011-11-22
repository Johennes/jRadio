/*
 * Settings.cpp
 * 
 * Copyright 2011 Johannes Marbach <JohannesMarbach@googlemail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "Settings.h"

#include <cstdlib>


namespace jRadio
{
    
    
    // Constructor & destructor
    
    Settings::Settings()
    {
    }
    
    Settings::~Settings()
    {
    }
    
    
    // Static member functions
    
    std::string Settings::default_icon_path()
    {
        return std::string(DATADIR) + std::string("/station.png");
    }
    
    std::string Settings::user_data_dir()
    {
        return std::string(getenv("HOME")) + std::string("/.jRadio");
    }
    
    std::string Settings::station_map_path()
    {
        return user_data_dir() + std::string("/stations.xml");
    }
    
    std::string Settings::vlc_log_path()
    {
        return user_data_dir() + std::string("/vlc.log");
    }
    
    
} // namespace jRadio
