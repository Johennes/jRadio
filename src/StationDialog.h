//
//
//      StationDialog.h
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
