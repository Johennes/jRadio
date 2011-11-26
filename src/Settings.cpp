//////////////////////////////////////////////////////////////////////////////
// Settings.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


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
