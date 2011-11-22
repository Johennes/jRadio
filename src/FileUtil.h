//
//
//      FileUtil.h
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



#ifndef H_FILE_UTIL
#define H_FILE_UTIL


#include <string>


namespace jRadio
{
    
    
    class FileUtil
    {
        public:
            FileUtil();
            virtual ~FileUtil();
            
            static std::string get_extension(std::string path);
            static std::string get_internal_filename(int ID, std::string path);
            static void copy_file(std::string source, std::string destination);
            static void remove_file(std::string path);
            static bool file_exists(std::string path);
            static void create_directory(std::string directory);
    };
    
    
} // namespace jRadio


#endif // H_FILE_UTIL
