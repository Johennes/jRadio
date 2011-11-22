//
//
//      AboutDialog.cpp
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
