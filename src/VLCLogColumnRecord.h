//////////////////////////////////////////////////////////////////////////////
// VLCLogColumnRecord.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.



#ifndef H_VLC_LOG_COLUMN_RECORD
#define H_VLC_LOG_COLUMN_RECORD


#include <gtkmm/treemodel.h>


namespace jRadio
{
    
    
    class VLCLogColumnRecord : public Gtk::TreeModelColumnRecord
    {
        public:
            VLCLogColumnRecord();
            virtual ~VLCLogColumnRecord();
            
            Gtk::TreeModelColumn<Glib::ustring> timeColumn;
            Gtk::TreeModelColumn<Glib::ustring> messageColumn;
    };
    
    
} // namespace jRadio


#endif // H_VLC_LOG_COLUMN_RECORD
