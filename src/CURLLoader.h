//////////////////////////////////////////////////////////////////////////////
// CURLLoader.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



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
