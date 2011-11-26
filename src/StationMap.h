//////////////////////////////////////////////////////////////////////////////
// StationMap.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



#ifndef H_STATION_MAP
#define H_STATION_MAP


#include <map>
#include <string>

#include <libxml/xmlstring.h>

#include "Station.h"


namespace jRadio
{
    
    
    class StationMap : public std::map<int, Station>
    {
        public:
            StationMap();
            virtual ~StationMap();
            
            void read_from_file(std::string file);
            void write_to_file(std::string file) const;
            xmlChar* convert_input(std::string input) const;
    };
    
    
} // namespace jRadio


#endif // H_STATION_MAP
