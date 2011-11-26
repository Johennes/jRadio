//////////////////////////////////////////////////////////////////////////////
// AboutDialog.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



#ifndef H_ABOUT_DIALOG
#define H_ABOUT_DIALOG


#include <gtkmm/aboutdialog.h>


namespace jRadio
{
    
    
    class AboutDialog : public Gtk::AboutDialog
    {
        public:
            AboutDialog(Gtk::Window& parent);
            virtual ~AboutDialog();
    };
    
    
} // namespace jRadio


#endif // H_ABOUT_DIALOG
