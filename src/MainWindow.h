//
//
//      MainWindow.h
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


#ifndef H_MAIN_WINDOW
#define H_MAIN_WINDOW


#include "PlaylistHistoryColumnRecord.h"
#include "StationListColumnRecord.h"
#include "StationMap.h"
#include "VLC.h"
#include "VLCLogColumnRecord.h"

#include <string>

#include <gtkmm/actiongroup.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/volumebutton.h>
#include <gtkmm/window.h>


namespace jRadio
{
    
    
    typedef enum {
        PlayerModePlaying,
        PlayerModePaused,
        PlayerModeStopped,
        PlayerModeNone
    } PlayerMode;
    
    class MainWindow : public Gtk::Window
    {
        public:
            MainWindow();
            virtual ~MainWindow();
        
        protected:
            void set_station_widgets_sensitive(bool sensitive);
            void set_playback_button_sensitivity(PlayerMode mode);
            void update_player_station_widgets();
            void update_player_track_labels();
            void update_playlist_history(std::string meta);
            bool update_player_time_label();
            
            void play(Station& station);
            void resume();
            void pause();
            void stop();
            void fail();
            
            void prepare_to_quit();
            Gtk::TreeIter get_selected_station_iter();
            Station* get_selected_station();
            Station* get_selected_station(Gtk::TreeIter iter);
            Gtk::TreeIter insert_station(Station station);
            
            bool on_delete_event(GdkEventAny* event);
            void on_save_action_activated();
            void on_quit_action_activated();
            void on_add_station_action_activated();
            void on_remove_station_action_activated();
            void on_edit_station_action_activated();
            void on_play_action_activated();
            void on_pause_action_activated();
            void on_stop_action_activated();
            void on_about_action_activated();
            void on_player_volume_button_value_changed(double value);
            void on_player_play_button_clicked();
            void on_player_pause_button_clicked();
            void on_player_stop_button_clicked();
            void on_station_list_tree_view_row_activated(const Gtk::TreeModel::Path& path,
                                                         Gtk::TreeViewColumn* column);
            void on_station_list_tree_view_selection_changed();
            void on_playlist_history_scrolled_window_vadjustment_value_changed();
            void on_playlist_history_tree_view_size_allocate(Gtk::Allocation& allocation);
            void on_vlc_log_scrolled_window_vadjustment_value_changed();
            void on_vlc_log_tree_view_size_allocate(Gtk::Allocation& allocation);
            void on_media_meta_changed(libvlc_meta_t meta_type);
            void on_player_encountered_error();
            void on_vlc_log_line_available(Glib::ustring line);
            
            Glib::RefPtr<Gtk::UIManager> rUIManager;
            Glib::RefPtr<Gtk::ActionGroup> rActionGroup;
            
            Gtk::VBox mainVBox;
            
            Gtk::Grid playerGrid;
            
            Gtk::Image playerIcon;
            Gtk::Label playerStationLabel;
            Gtk::Label playerNowPlayingLabel;
            Gtk::Label playerTimeLabel;
            Gtk::VolumeButton playerVolumeButton;
            Gtk::Button playerPlayButton;
            Gtk::Image playerPlayButtonImage;
            Gtk::Button playerPauseButton;
            Gtk::Image playerPauseButtonImage;
            Gtk::Button playerStopButton;
            Gtk::Image playerStopButtonImage;
            
            Gtk::VPaned mainVPaned;
            
            Gtk::ScrolledWindow stationListScrolledWindow;
            Gtk::TreeView stationListTreeView;
            StationListColumnRecord stationListTreeViewColumns;
            Glib::RefPtr<Gtk::ListStore> pStationListStore;
            StationMap stations;
            
            Gtk::Notebook infoNotebook;
            Gtk::ScrolledWindow playlistHistoryScrolledWindow;
            Gtk::TreeView playlistHistoryTreeView;
            PlaylistHistoryColumnRecord playlistHistoryTreeViewColumns;
            Glib::RefPtr<Gtk::ListStore> pPlaylistHistoryListStore;
            Gtk::ScrolledWindow vlcLogScrolledWindow;
            Gtk::TreeView vlcLogTreeView;
            VLCLogColumnRecord vlcLogTreeViewColumns;
            Glib::RefPtr<Gtk::ListStore> pVlcLogListStore;
            
            bool playlistHistoryTreeViewShallScrollToBottom;
            bool vlcLogTreeViewShallScrollToBottom;
            
            VLC* pVlc;
            
            Station* pCurrentStation;
            time_t currentSongStart;
            
            sigc::connection trackTimeConnection;
    };
    
    
} // namespace jRadio


#endif // H_MAIN_WINDOW
