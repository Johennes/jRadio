//////////////////////////////////////////////////////////////////////////////
// AboutDialog.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#include <config.h>

#include "AboutDialog.h"


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
        set_license_type (Gtk::LICENSE_LGPL_3_0);
        
        std::vector<Glib::ustring> authors;
        authors.push_back("Johannes Marbach");
        set_authors(authors);
    }
    
    AboutDialog::~AboutDialog()
    {
    }
    
    
} // namespace jRadio
