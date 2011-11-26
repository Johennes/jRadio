//////////////////////////////////////////////////////////////////////////////
// FileUtil.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


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
