//
//
//      FileUtil.cpp
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


#include "FileUtil.h"

#include "Settings.h"

#include <algorithm>
#include <sstream>

#include <giomm/file.h>


namespace jRadio
{
    
    
    // Constructor & destructor
    
    FileUtil::FileUtil()
    {
    }
    
    FileUtil::~FileUtil()
    {
    }
    
    
    // Static member functions
    
    std::string FileUtil::get_extension(std::string path)
    {
        size_t idxDot = path.find_last_of(".");
        size_t idxSlash = path.find_last_of("/");
        
        if (idxDot == std::string::npos || idxDot < idxSlash)
        {
            return "";
        }
        else
        {
            std::string extension = path.substr(idxDot + 1); // Extract extension
            
            // Convert to lowercase string
            const int length = extension.length();
            for (int i=0; i < length; ++i)
            {
                extension[i] = std::tolower(extension[i]);
            }
            
            return extension;
        }
    }
    
    std::string FileUtil::get_internal_filename(int ID, std::string path)
    {
        std::ostringstream s;
        
        s << Settings::user_data_dir() << "/" << ID;
        
        std::string extension = get_extension(path);
        if (extension.length())
        {
            s << "." << extension;
        }
        
        return s.str();
    }
    
    void FileUtil::copy_file(std::string source, std::string destination)
    {
        Glib::RefPtr<Gio::File> fileSource = Gio::File::create_for_path(source);
        Glib::RefPtr<Gio::File> fileDestination = Gio::File::create_for_path(destination);
        fileSource->copy(fileDestination, Gio::FILE_COPY_OVERWRITE);
    }
    
    void FileUtil::remove_file(std::string path)
    {
        if (! path.length())
        {
            return;
        }
        
        Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(path);
        if (! file->query_exists())
        {
            return;
        }
        
        file->remove();
    }
    
    bool FileUtil::file_exists(std::string path)
    {
        Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(path);
        return file->query_exists();
    }
    
    void FileUtil::create_directory(std::string directory)
    {
        Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(directory);
        file->make_directory_with_parents();
    }
    
    
} // namespace jRadio
