//
//
//      CURLLoader.cpp
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


#include "CURLLoader.h"

#include <stdexcept>

#include <curl/curl.h>


namespace jRadio
{
    
    
    // Constructor & destructor
    
    CURLLoader::CURLLoader()
    {
    }
    
    CURLLoader::~CURLLoader()
    {
    }
    
    
    // Static member functions
    
    void CURLLoader::download(std::string URL, std::string destination)
    {
        int error = 0;
        
        // Initialize CURL
        curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* pCURL = curl_easy_init();
        
        if (pCURL)
        {
            // Open destination file
            FILE* fp = fopen(destination.c_str(), "wb");
            if (! fp)
            {
                error = 1;
            }
            else
            {
                // Setup CURL options
                curl_easy_setopt(pCURL, CURLOPT_URL, URL.c_str());
                curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, CURLLoader::write);
                curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, fp);
                
                CURLcode res = curl_easy_perform(pCURL); // Perform download
                
                curl_easy_cleanup(pCURL); // Memory clean-up
                
                fclose(fp); // Close destination file
                
                if (res != CURLE_OK)
                {
                    error = 2;
                }
            }
        }
        
        curl_global_cleanup(); // Global memory clean-up
        
        if (error)
        {
            throw std::runtime_error("CURL download failed.");
        }
    }
    
    size_t CURLLoader::write(void* buffer, size_t size, size_t nmemb, FILE* fp)
    {
        return fwrite(buffer, size, nmemb, fp);
    }
 
    
} // namespace jRadio
