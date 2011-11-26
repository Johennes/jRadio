//////////////////////////////////////////////////////////////////////////////
// StationDialog.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



#ifndef H_STATION_DIALOG
#define H_STATION_DIALOG


#include "Station.h"

#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>


namespace jRadio
{
    
    
    class StationDialog : public Gtk::Dialog
    {
        public:
            StationDialog(Gtk::Window& parent, Station& station);
            virtual ~StationDialog();
            
            std::string get_station_title();
            std::string get_station_genre();
            std::string get_station_mrl();
            
            void update_icon();
            
            void on_icon_button_clicked();
        
        protected:
            Gtk::Grid mainGrid;
            Gtk::Image icon;
            Gtk::Button iconButton;
            Gtk::Label titleLabel;
            Gtk::Entry titleEntry;
            Gtk::Label genreLabel;
            Gtk::Entry genreEntry;
            Gtk::Label MRLLabel;
            Gtk::Entry MRLEntry;
            
            Station& rStation;
    };
    
    
} // namespace jRadio


#endif // H_STATION_DIALOG
