//
//
//      StationMap.h
//      
//      Copyright 2011 Johannes Marbach <JohannesMarbach@googlemail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//      



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
