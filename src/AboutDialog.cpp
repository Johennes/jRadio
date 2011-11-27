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
        set_license (
            "Copyright 2011 Johannes Marbach. All rights reserved.\n"
            "\n"
            "Redistribution and use in source and binary forms, with or without modification, are\n"
            "permitted provided that the following conditions are met:\n"
            "\n"
            "1. Redistributions of source code must retain the above copyright notice, this list of\n"
            "conditions and the following disclaimer.\n"
            "\n"
            "2. Redistributions in binary form must reproduce the above copyright notice, this list\n"
            "of conditions and the following disclaimer in the documentation and/or other materials\n"
            "provided with the distribution.\n"
            "\n"
            "THIS SOFTWARE IS PROVIDED BY JOHANNES MARBACH ''AS IS'' AND ANY EXPRESS OR IMPLIED\n"
            "WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND\n"
            "FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL JOHANNES MARBACH OR\n"
            "CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n"
            "CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"
            "SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON\n"
            "ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n"
            "NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF\n"
            "ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
            "\n"
            "The views and conclusions contained in the software and documentation are those of the\n"
            "authors and should not be interpreted as representing official policies, either expressed\n"
            "or implied, of Johannes Marbach.\n"
        );
        
        std::vector<Glib::ustring> authors;
        authors.push_back("Johannes Marbach");
        set_authors(authors);
    }
    
    AboutDialog::~AboutDialog()
    {
    }
    
    
} // namespace jRadio
