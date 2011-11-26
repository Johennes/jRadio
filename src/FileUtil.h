//////////////////////////////////////////////////////////////////////////////
// FileUtil.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



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
