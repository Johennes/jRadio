//////////////////////////////////////////////////////////////////////////////
// AboutDialog.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#include <config.h>

#include "AboutDialog.h"

#include <iostream>
#include <fstream>


namespace jRadio
{
    
    
    // Constructor & destructor
    
    AboutDialog::AboutDialog(Gtk::Window& parent)
    {
        set_transient_for(parent); // Setup parent window
        
        // Insert program info
        set_program_name(PACKAGE_NAME);
        set_version(PACKAGE_VERSION);
        set_copyright("© 2011 Johannes Marbach");
        set_comments("An easy to use web radio player.");
        
        set_license_type (Gtk::LICENSE_CUSTOM);
        
        std::ifstream license("LICENSE");
        if (license.is_open())
        {
            std::stringstream licenseStream;
            licenseStream << license.rdbuf();
            license.close();
            
            set_license (licenseStream.str());
        }
        
        std::vector<Glib::ustring> authors;
        authors.push_back("Johannes Marbach");
        set_authors(authors);
    }
    
    AboutDialog::~AboutDialog()
    {
    }
    
    
} // namespace jRadio
