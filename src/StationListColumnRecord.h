//////////////////////////////////////////////////////////////////////////////
// StationListColumnRecord.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



#ifndef H_STATION_LIST_COLUMN_RECORD
#define H_STATION_LIST_COLUMN_RECORD


#include <gtkmm/treemodel.h>


namespace jRadio
{
    
    
    class StationListColumnRecord : public Gtk::TreeModelColumnRecord
    {
        public:
            StationListColumnRecord();
            virtual ~StationListColumnRecord();
            
            Gtk::TreeModelColumn<unsigned int> IDColumn;
            Gtk::TreeModelColumn<Glib::ustring> titleColumn;
            Gtk::TreeModelColumn<Glib::ustring> genreColumn;
    };
    
    
} // namespace jRadio


#endif // H_STATION_LIST_COLUMN_RECORD
