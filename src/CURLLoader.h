//
//
//      CURLLoader.h
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



#ifndef H_CURL_LOADER
#define H_CURL_LOADER


#include <string>


namespace jRadio
{
    
    
    class CURLLoader
    {
        public:
            CURLLoader();
            virtual ~CURLLoader();
            
            static void download(std::string URL, std::string destination);
        
        protected:
            static size_t write(void* buffer, size_t size, size_t nmemb, FILE* fp);
    };
    
    
} // namespace jRadio


#endif // H_CURL_LOADER
