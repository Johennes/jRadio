//////////////////////////////////////////////////////////////////////////////
// Settings.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



#ifndef H_SETTINGS
#define H_SETTINGS


#include <string>


namespace jRadio
{
    
    
    class Settings
    {
        public:
            Settings();
            virtual ~Settings();
            
            static std::string default_icon_path();
            static std::string user_data_dir();
            static std::string station_map_path();
            static std::string vlc_log_path();
    };
    
    
} // namespace jRadio


#endif // H_SETTINGS
