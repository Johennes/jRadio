//////////////////////////////////////////////////////////////////////////////
// Station.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



#ifndef H_STATION
#define H_STATION


#include <string>


namespace jRadio
{
    
    
    class Station
    {
        public:
            Station();
            virtual ~Station();
            
            int ID;
            std::string title;
            std::string genre;
            std::string MRL;
            std::string iconPath;
    };
    
    
} // namespace jRadio


#endif // H_STATION
