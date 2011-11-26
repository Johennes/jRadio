//////////////////////////////////////////////////////////////////////////////
// PlaylistHistoryColumnRecord.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



#ifndef H_PLAYLIST_HISTORY_COLUMN_RECORD
#define H_PLAYLIST_HISTORY_COLUMN_RECORD


#include <gtkmm/treemodel.h>


namespace jRadio
{
    
    
    class PlaylistHistoryColumnRecord : public Gtk::TreeModelColumnRecord
    {
        public:
            PlaylistHistoryColumnRecord();
            virtual ~PlaylistHistoryColumnRecord();
            
            Gtk::TreeModelColumn<Glib::ustring> timeColumn;
            Gtk::TreeModelColumn<Glib::ustring> stationColumn;
            Gtk::TreeModelColumn<Glib::ustring> metaColumn;
    };
    
    
} // namespace jRadio


#endif // H_PLAYLIST_HISTORY_COLUMN_RECORD
