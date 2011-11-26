//////////////////////////////////////////////////////////////////////////////
// MainWindow.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#include "MainWindow.h"

#include "AboutDialog.h"
#include "FileUtil.h"
#include "Settings.h"
#include "StationDialog.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <gtkmm/stock.h>


namespace jRadio
{
    
    
    // Constructor & destructor
    
    MainWindow::MainWindow() :
        playerPlayButtonImage(Gtk::Stock::MEDIA_PLAY, Gtk::ICON_SIZE_BUTTON),
        playerPauseButtonImage(Gtk::Stock::MEDIA_PAUSE, Gtk::ICON_SIZE_BUTTON),
        playerStopButtonImage(Gtk::Stock::MEDIA_STOP, Gtk::ICON_SIZE_BUTTON),
        playlistHistoryTreeViewShallScrollToBottom(true),
        vlcLogTreeViewShallScrollToBottom(true),
        pCurrentStation(0),
        currentSongStart(0)
    {
        // Initialize user data dir
        if (! FileUtil::file_exists(Settings::user_data_dir()))
        {
            FileUtil::create_directory(Settings::user_data_dir());
        }
        
        // Setup window properties
        set_title("jRadio");
        set_size_request(600, 400);
        resize(800, 600);
        
        add(mainVBox); // Insert main VBox
        
        // Initialize & populate action group
        rActionGroup = Gtk::ActionGroup::create();
        
        // File menu
        rActionGroup->add(Gtk::Action::create("FileMenu", "File"));
        rActionGroup->add(Gtk::Action::create("SaveAction", Gtk::Stock::SAVE, "Save station list",
            "Save station list"), sigc::mem_fun(*this, &MainWindow::on_save_action_activated));
        rActionGroup->add(Gtk::Action::create("QuitAction", Gtk::Stock::QUIT, "", "Quit jRadio"),
            sigc::mem_fun(*this, &MainWindow::on_quit_action_activated));
        
        // Edit menu
        rActionGroup->add(Gtk::Action::create("EditMenu", "Edit"));
        rActionGroup->add(Gtk::Action::create("AddStationAction", Gtk::Stock::ADD, "Add station",
            "Add a new station"), sigc::mem_fun(*this, &MainWindow::on_add_station_action_activated));
        rActionGroup->add(Gtk::Action::create("RemoveStationAction", Gtk::Stock::REMOVE, "Remove station",
            "Delete the selected station"), sigc::mem_fun(*this, &MainWindow::on_remove_station_action_activated));
        rActionGroup->add(Gtk::Action::create("EditStationAction", Gtk::Stock::EDIT, "Edit station",
            "Edit selected station"), sigc::mem_fun(*this, &MainWindow::on_edit_station_action_activated));
        
        // Playback menu
        rActionGroup->add(Gtk::Action::create("PlaybackMenu", "Playback"));
        rActionGroup->add(Gtk::Action::create("PlayAction", Gtk::Stock::MEDIA_PLAY, "Play",
            "Play station"), sigc::mem_fun(*this, &MainWindow::on_play_action_activated));
        rActionGroup->add(Gtk::Action::create("PauseAction", Gtk::Stock::MEDIA_PAUSE, "Pause",
            "Play station"), sigc::mem_fun(*this, &MainWindow::on_pause_action_activated));
        rActionGroup->add(Gtk::Action::create("StopAction", Gtk::Stock::MEDIA_STOP, "Stop",
            "Stop playback"), sigc::mem_fun(*this, &MainWindow::on_stop_action_activated));
        
        // Help menu
        rActionGroup->add(Gtk::Action::create("HelpMenu", "Help"));
        rActionGroup->add(Gtk::Action::create("AboutAction", Gtk::Stock::ABOUT),
            sigc::mem_fun(*this, &MainWindow::on_about_action_activated));
        
        // Initialize & setup UI manager
        rUIManager = Gtk::UIManager::create();
        rUIManager->insert_action_group(rActionGroup);
        
        add_accel_group(rUIManager->get_accel_group()); // Connect keyboard shortcuts
        
        // Define action layout
        Glib::ustring UIString =
            "<ui>"
                "<menubar name='Menubar'>"
                    "<menu action='FileMenu'>"
                        "<menuitem action='SaveAction'/>"
                        "<separator/>"
                        "<menuitem action='QuitAction'/>"
                    "</menu>"
                    "<menu action='EditMenu'>"
                        "<menuitem action='AddStationAction'/>"
                        "<menuitem action='RemoveStationAction'/>"
                        "<menuitem action='EditStationAction'/>"
                    "</menu>"
                    "<menu action='PlaybackMenu'>"
                        "<menuitem action='PlayAction'/>"
                        "<menuitem action='PauseAction'/>"
                        "<menuitem action='StopAction'/>"
                    "</menu>"
                    "<menu action='HelpMenu'>"
                        "<menuitem action='AboutAction'/>"
                    "</menu>"
                "</menubar>"
                "<toolbar name='Toolbar'>"
                    "<toolitem action='SaveAction'/>"
                    "<separator/>"
                    "<toolitem action='AddStationAction'/>"
                    "<toolitem action='RemoveStationAction'/>"
                    "<toolitem action='EditStationAction'/>"
                    "<separator/>"
                    "<toolitem action='QuitAction'/>"
                "</toolbar>"
            "</ui>";
        
        try
        {
            rUIManager->add_ui_from_string(UIString);
        }
        catch (const Glib::Error& error)
        {
            std::cerr << "Initializing UI from string failed with error: " <<  error.what() << "\n";
        }
        
        // Extract menubar & insert it into the UI
        Gtk::Widget* pMenubar = rUIManager->get_widget("/Menubar");
        if (pMenubar) mainVBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);
        
        // Extract toolbar & insert it into the UI
        Gtk::Widget* pToolbar = rUIManager->get_widget("/Toolbar") ;
        if (pToolbar) mainVBox.pack_start(*pToolbar, Gtk::PACK_SHRINK);
        
        // Insert player grid
        mainVBox.pack_start(playerGrid, false, false);
        playerGrid.set_row_spacing(10);
        playerGrid.set_column_spacing(10);
        playerGrid.set_margin_top(20);
        playerGrid.set_margin_right(20);
        playerGrid.set_margin_bottom(20);
        playerGrid.set_margin_left(20);
        
        // Insert child widgets
        playerGrid.attach(playerIcon, 0, 0, 1, 4);
        playerGrid.attach(playerStationLabel, 1, 1, 1, 1);
        playerGrid.attach(playerNowPlayingLabel, 1, 2, 1, 1);
        
        playerStationLabel.set_halign(Gtk::ALIGN_START);
        playerNowPlayingLabel.set_halign(Gtk::ALIGN_START);
        
        playerStationLabel.set_hexpand(true);
        playerStationLabel.set_vexpand(false);
        playerNowPlayingLabel.set_hexpand(true);
        playerNowPlayingLabel.set_vexpand(false);
        
        playerStationLabel.set_margin_left(15);
        playerStationLabel.set_margin_right(15);
        playerNowPlayingLabel.set_margin_left(15);
        playerNowPlayingLabel.set_margin_right(15);
        
        playerStationLabel.set_ellipsize(Pango::ELLIPSIZE_END);
        playerNowPlayingLabel.set_ellipsize(Pango::ELLIPSIZE_END);
        
        
        playerGrid.attach(playerTimeLabel, 2, 1, 2, 1);
        playerGrid.attach(playerVolumeButton, 4, 1, 1, 1);
        
        playerTimeLabel.set_hexpand(false);
        playerTimeLabel.set_vexpand(false);
        
        playerTimeLabel.set_text("0:00");
        playerVolumeButton.set_value(100);
        
        playerVolumeButton.signal_value_changed().connect(sigc::mem_fun(*this,
            &MainWindow::on_player_volume_button_value_changed));
        
        
        playerPlayButton.add(playerPlayButtonImage);
        playerPlayButton.signal_clicked().connect(sigc::mem_fun(*this,
            &MainWindow::on_player_play_button_clicked));
        
        playerPauseButton.add(playerPauseButtonImage);
        playerPauseButton.signal_clicked().connect(sigc::mem_fun(*this,
            &MainWindow::on_player_pause_button_clicked));
        
        playerStopButton.add(playerStopButtonImage);
        playerStopButton.signal_clicked().connect(sigc::mem_fun(*this,
            &MainWindow::on_player_stop_button_clicked));
        
        playerGrid.attach(playerPlayButton, 2, 2, 1, 1);
        playerGrid.attach(playerPauseButton, 3, 2, 1, 1);
        playerGrid.attach(playerStopButton, 4, 2, 1, 1);
        
        playerPlayButton.set_hexpand(false);
        playerPlayButton.set_vexpand(false);
        playerPauseButton.set_hexpand(false);
        playerPauseButton.set_vexpand(false);
        playerStopButton.set_hexpand(false);
        playerStopButton.set_vexpand(false);
        
        // Insert main VPaned
        mainVBox.pack_start(mainVPaned, true, true);
        mainVPaned.set_position(250);
        
        // Insert station tree view
        mainVPaned.pack1(stationListScrolledWindow, true, true);
        stationListScrolledWindow.add(stationListTreeView);
        stationListScrolledWindow.set_vexpand(true);
        
        // Create station list store
        pStationListStore = Gtk::ListStore::create (stationListTreeViewColumns);
        
        // Load stations & populate list store
        stations.read_from_file(Settings::station_map_path());
        pStationListStore->set_sort_column(stationListTreeViewColumns.titleColumn, Gtk::SORT_ASCENDING);
        
        for (StationMap::iterator iter = stations.begin(); iter != stations.end(); ++iter)
        {
            insert_station(iter->second);
        }
        
        // Setup station tree view
        stationListTreeView.set_model(pStationListStore);
        stationListTreeView.append_column("Title", stationListTreeViewColumns.titleColumn);
        stationListTreeView.append_column("Genre", stationListTreeViewColumns.genreColumn);
        
        Gtk::TreeView::Column* pColumn = stationListTreeView.get_column(0);
        if (pColumn)
        {
            pColumn->set_sort_column(stationListTreeViewColumns.titleColumn);
        }
        
        pColumn = stationListTreeView.get_column(1);
        if (pColumn)
        {
            pColumn->set_sort_column(stationListTreeViewColumns.genreColumn);
        }
        
        stationListTreeView.signal_row_activated().connect(sigc::mem_fun(*this,
            &MainWindow::on_station_list_tree_view_row_activated));
        stationListTreeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this,
            &MainWindow::on_station_list_tree_view_selection_changed));
        
        // Add info notebook
        mainVPaned.pack2(infoNotebook, false, true);
        
        // Setup playlist history list store & tree view
        playlistHistoryScrolledWindow.add(playlistHistoryTreeView);
        infoNotebook.append_page(playlistHistoryScrolledWindow, "Playlist history");
        
        playlistHistoryTreeView.set_headers_visible(false);
        playlistHistoryTreeView.set_rules_hint(true);
        
        pPlaylistHistoryListStore = Gtk::ListStore::create (playlistHistoryTreeViewColumns);
        playlistHistoryTreeView.set_model(pPlaylistHistoryListStore);
        playlistHistoryTreeView.append_column("", playlistHistoryTreeViewColumns.timeColumn);
        playlistHistoryTreeView.append_column("", playlistHistoryTreeViewColumns.stationColumn);
        playlistHistoryTreeView.append_column("", playlistHistoryTreeViewColumns.metaColumn);
        
        playlistHistoryScrolledWindow.get_vadjustment()->signal_value_changed().connect(sigc::mem_fun(*this,
            &MainWindow::on_playlist_history_scrolled_window_vadjustment_value_changed));
        playlistHistoryTreeView.signal_size_allocate().connect(sigc::mem_fun(*this,
            &MainWindow::on_playlist_history_tree_view_size_allocate));
        
        // Setup VLC log list store & tree view
        vlcLogScrolledWindow.add(vlcLogTreeView);
        infoNotebook.append_page(vlcLogScrolledWindow, "VLC Log");
        
        vlcLogTreeView.set_headers_visible(false);
        vlcLogTreeView.set_rules_hint(true);
        
        pVlcLogListStore = Gtk::ListStore::create (vlcLogTreeViewColumns);
        vlcLogTreeView.set_model(pVlcLogListStore);
        vlcLogTreeView.append_column("", vlcLogTreeViewColumns.timeColumn);
        vlcLogTreeView.append_column("", vlcLogTreeViewColumns.messageColumn);
        
        vlcLogScrolledWindow.get_vadjustment()->signal_value_changed().connect(sigc::mem_fun(*this,
            &MainWindow::on_vlc_log_scrolled_window_vadjustment_value_changed));
        vlcLogTreeView.signal_size_allocate().connect(sigc::mem_fun(*this,
            &MainWindow::on_vlc_log_tree_view_size_allocate));
        
        set_station_widgets_sensitive(false); // Initially disable station widgets
        
        // Initially disable player
        update_player_station_widgets();
        set_playback_button_sensitivity(PlayerModeNone);
        
        // Initialize VLC instance
        pVlc = new VLC(Settings::vlc_log_path());
        pVlc->set_volume((int)(playerVolumeButton.get_value() * 100.0));
        
        // Connect to VLC events
        pVlc->media_meta_changed_signal.connect(sigc::mem_fun(*this, &MainWindow::on_media_meta_changed));
        pVlc->player_encountered_error_signal.connect(sigc::mem_fun(*this, &MainWindow::on_player_encountered_error));
        pVlc->log_line_available_signal.connect(sigc::mem_fun(*this, &MainWindow::on_vlc_log_line_available));
        
        show_all_children();
    }
    
    MainWindow::~MainWindow()
    {
        delete pVlc;
    }
    
    
    // UI updating methods
    
    void MainWindow::set_station_widgets_sensitive(bool sensitive)
    {
        rActionGroup->get_action("RemoveStationAction")->set_sensitive(sensitive);
        rActionGroup->get_action("EditStationAction")->set_sensitive(sensitive);
    }
    
    void MainWindow::set_playback_button_sensitivity(PlayerMode mode)
    {
        if (mode == PlayerModePlaying)
        {
            playerPlayButton.set_sensitive(false);
            playerPauseButton.set_sensitive(true);
            playerStopButton.set_sensitive(true);
            
            rActionGroup->get_action("PlayAction")->set_sensitive(false);
            rActionGroup->get_action("PauseAction")->set_sensitive(true);
            rActionGroup->get_action("StopAction")->set_sensitive(true);
        }
        else if (mode == PlayerModePaused)
        {
            playerPlayButton.set_sensitive(true);
            playerPauseButton.set_sensitive(false);
            playerStopButton.set_sensitive(true);
            
            rActionGroup->get_action("PlayAction")->set_sensitive(true);
            rActionGroup->get_action("PauseAction")->set_sensitive(false);
            rActionGroup->get_action("StopAction")->set_sensitive(true);
        }
        else if (mode == PlayerModeStopped)
        {
            playerPlayButton.set_sensitive(false);
            playerPauseButton.set_sensitive(false);
            playerStopButton.set_sensitive(false);
            
            rActionGroup->get_action("PlayAction")->set_sensitive(false);
            rActionGroup->get_action("PauseAction")->set_sensitive(false);
            rActionGroup->get_action("StopAction")->set_sensitive(false);
        }
        else if (mode == PlayerModeNone)
        {
            playerPlayButton.set_sensitive(false);
            playerPauseButton.set_sensitive(false);
            playerStopButton.set_sensitive(false);
            
            rActionGroup->get_action("PlayAction")->set_sensitive(false);
            rActionGroup->get_action("PauseAction")->set_sensitive(false);
            rActionGroup->get_action("StopAction")->set_sensitive(false);
        }
    }
    
    void MainWindow::update_player_station_widgets()
    {
        if (pCurrentStation)
        {
            playerStationLabel.set_text(pCurrentStation->title);
            
            try
            {
                Glib::RefPtr<Gdk::Pixbuf> pPixbuf =
                    Gdk::Pixbuf::create_from_file(pCurrentStation->iconPath, 64, 64);
                playerIcon.set(pPixbuf);
            }
            catch (...)
            {
                FileUtil::remove_file(pCurrentStation->iconPath);
                pCurrentStation->iconPath = "";
                
                Glib::RefPtr<Gdk::Pixbuf> pPixbuf =
                    Gdk::Pixbuf::create_from_file(Settings::default_icon_path(), 64, 64);
                playerIcon.set(pPixbuf);
                playerIcon.set(pPixbuf);
            }
        }
        else
        {
            playerStationLabel.set_markup("<i>No station selected</i>");
            playerNowPlayingLabel.set_markup("<i>player stopped</i>");
            
            Glib::RefPtr<Gdk::Pixbuf> pPixbuf =
                Gdk::Pixbuf::create_from_file(Settings::default_icon_path(), 64, 64);
            playerIcon.set(pPixbuf);
        }
    }
    
    void MainWindow::update_player_track_labels()
    {
        std::string newMeta = pVlc->get_meta_now_playing();
        std::string oldMeta = playerNowPlayingLabel.get_text();
        
        if (newMeta != oldMeta)
        {
            if (pCurrentStation)
            {
                update_playlist_history(oldMeta);
            }
            
            playerNowPlayingLabel.set_text(newMeta);
            playerTimeLabel.set_text("0:00");
        }
    }
    
    void MainWindow::update_playlist_history(std::string meta)
    {
        static std::string lastStation = "";
        static std::string lastMeta = "";
        
        time_t rawTime = time(0); // Get current time
        
        if (meta.length() > 0 && currentSongStart > 0 && rawTime - currentSongStart >= 60)
        {
            // Get previous playlist item
            if (lastStation != pCurrentStation->title || lastMeta != meta)
            {
                // Get (old) starting time as string
                struct tm* timeInfo = localtime(&currentSongStart);
                char timeString[80];
                strftime (timeString, 80, "%H:%M", timeInfo);
                
                // Insert new row in playlist history list store
                Gtk::TreeIter iter = pPlaylistHistoryListStore->append();
                Gtk::TreeModel::Row row = *iter;
                
                row[playlistHistoryTreeViewColumns.timeColumn] = timeString;
                row[playlistHistoryTreeViewColumns.stationColumn] = pCurrentStation->title;
                row[playlistHistoryTreeViewColumns.metaColumn] = meta;
                
                // Remember last inserted station & meta
                lastStation = pCurrentStation->title;
                lastMeta = meta;
            }
        }
        
        currentSongStart = rawTime; // Store new song starting time
    }
    
    bool MainWindow::update_player_time_label()
    {
        if (currentSongStart && playerNowPlayingLabel.get_text().length())
        {
            int secs = time(0) - currentSongStart;
            int mins = secs / 60;
            secs -= 60 * mins;
            
            std::ostringstream s;
            s << mins << ":" << std::setfill('0') << std::setw(2) << secs;
            playerTimeLabel.set_text(s.str());
        }
        
        return true;
    }
    
    
    // Playback methods
    
    void MainWindow::play(Station& station)
    {
        pCurrentStation = &station;
        
        update_player_station_widgets();
        set_playback_button_sensitivity(PlayerModePlaying);
        
        pVlc->play(station.MRL);
        
        trackTimeConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this,
            &MainWindow::update_player_time_label), 1000);
    }
    
    void MainWindow::resume()
    {
        set_playback_button_sensitivity(PlayerModePlaying);
        pVlc->resume();
        update_player_track_labels();
        
        trackTimeConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this,
            &MainWindow::update_player_time_label), 1000);
    }
    
    void MainWindow::pause()
    {
        trackTimeConnection.disconnect();
        playerTimeLabel.set_text("0:00");
        
        pVlc->pause();
        
        update_playlist_history(playerNowPlayingLabel.get_text());
        currentSongStart = 0;
        
        playerNowPlayingLabel.set_markup("<i>player paused</i>");
        set_playback_button_sensitivity(PlayerModePaused);
    }
    
    void MainWindow::stop()
    {
        trackTimeConnection.disconnect();
        playerTimeLabel.set_text("0:00");
        
        pVlc->stop();
        
        update_playlist_history(playerNowPlayingLabel.get_text());
        
        pCurrentStation = 0;
        currentSongStart = 0;
        
        update_player_station_widgets();
        set_playback_button_sensitivity(PlayerModeStopped);
    }
    
    void MainWindow::fail()
    {
        currentSongStart = 0;
        playerNowPlayingLabel.set_markup("<i>player failed to open stream</i>");
        set_playback_button_sensitivity(PlayerModeNone);
    }
    
    
    // Miscellaneous methods
    
    void MainWindow::prepare_to_quit()
    {
        stations.write_to_file(Settings::station_map_path());
        
        pVlc->shutdown();
        FileUtil::remove_file(Settings::vlc_log_path());
    }
    
    Gtk::TreeIter MainWindow::get_selected_station_iter()
    {
        return stationListTreeView.get_selection()->get_selected();
    }
    
    Station* MainWindow::get_selected_station()
    {
        return get_selected_station(get_selected_station_iter());
    }
    
    Station* MainWindow::get_selected_station(Gtk::TreeIter iter)
    {
        if (! iter)
        {
            return 0;
        }
        else
        {
            int ID = (*iter)[stationListTreeViewColumns.IDColumn];
            return &stations[ID];
        }
    }
    
    Gtk::TreeIter MainWindow::insert_station(Station station)
    {
        Gtk::TreeIter iter = pStationListStore->append();
        Gtk::TreeModel::Row row = *iter;
        
        row[stationListTreeViewColumns.IDColumn] = station.ID;
        row[stationListTreeViewColumns.titleColumn] = station.title;
        row[stationListTreeViewColumns.genreColumn] = station.genre;
        
        return iter;
    }
    
    
    // Event handlers
    
    bool MainWindow::on_delete_event(GdkEventAny* event) {
        prepare_to_quit();
        return false;
    }
    
    void MainWindow::on_save_action_activated()
    {
        stations.write_to_file(Settings::station_map_path());
    }
    
    void MainWindow::on_quit_action_activated()
    {
        prepare_to_quit();
        hide();
    }
    
    void MainWindow::on_add_station_action_activated()
    {
        // Query lowest unused ID
        int ID = 0;
        for (StationMap::iterator iter = stations.begin(); iter != stations.end(); ++iter)
        {
            if (iter->first == ID) ++ID;
            else break;
        }
        
        // Create new station
        Station station;
        station.ID = ID;
        station.title = "New Station";
        
        // Load & run station dialog
        StationDialog dialog(*this, station);
        
        if (dialog.run() == Gtk::RESPONSE_OK)
        {
            // Update station details
            station.title = dialog.get_station_title();
            station.genre = dialog.get_station_genre();
            station.MRL = dialog.get_station_mrl();
            
            stations[ID] = station; // Store station in station map
            
            // Insert station into list store & select the associated row
            Gtk::TreeIter iter = insert_station(station);
            stationListTreeView.get_selection()->select(iter);
        }
        else
        {
            FileUtil::remove_file(station.iconPath);
        }
    }
    
    void MainWindow::on_remove_station_action_activated()
    {
        // Get selected iter & ID
        Gtk::TreeIter iter = get_selected_station_iter();
        Station* pStation = get_selected_station(iter);
        
        stationListTreeView.get_selection()->unselect_all(); // Clear selection
        
        // Remove station icon
        FileUtil::remove_file(pStation->iconPath);
        
        // Erase station from list store & station map
        pStationListStore->erase(iter);
        stations.erase(pStation->ID);
    }
    
    void MainWindow::on_edit_station_action_activated()
    {
        Station* pStation = get_selected_station();
        StationDialog dialog(*this, *pStation);
        
        // Backup current icon
        std::string iconPath = pStation->iconPath;
        if (iconPath.length())
        {
            FileUtil::copy_file(iconPath, Settings::user_data_dir() + std::string("/backup"));
        }
        
        if (dialog.run() == Gtk::RESPONSE_OK)
        {
            // Update station details
            pStation->title = dialog.get_station_title();
            pStation->genre = dialog.get_station_genre();
            pStation->MRL = dialog.get_station_mrl();
            
            // Update tree view row
            Gtk::TreeIter iter = get_selected_station_iter();
            (*iter)[stationListTreeViewColumns.titleColumn] = pStation->title;
            (*iter)[stationListTreeViewColumns.genreColumn] = pStation->genre;
            
            // Update player widgets
            if (pCurrentStation && pStation->ID == pCurrentStation->ID)
            {
                update_player_station_widgets();
            }
        }
        else if (iconPath.length() && pStation->iconPath != iconPath)
        {
            // Reset icon
            FileUtil::remove_file(pStation->iconPath);
            FileUtil::copy_file(Settings::user_data_dir() + std::string("/backup"), iconPath);
            pStation->iconPath = iconPath;
        }
        
        FileUtil::remove_file(Settings::user_data_dir() + std::string("/backup")); // Clean-up backup file
    }
    
    void MainWindow::on_play_action_activated()
    {
        resume();
    }
    
    void MainWindow::on_pause_action_activated()
    {
        pause();
    }
    
    void MainWindow::on_stop_action_activated()
    {
        stop();
    }
    
    void MainWindow::on_about_action_activated()
    {
        AboutDialog dialog(*this);
        dialog.run();
    }
    
    void MainWindow::on_player_volume_button_value_changed(double value)
    {
        pVlc->set_volume((int)(value * 100.0));
    }
    
    void MainWindow::on_player_play_button_clicked()
    {
        resume();
    }
    
    void MainWindow::on_player_pause_button_clicked()
    {
        pause();
    }
    
    void MainWindow::on_player_stop_button_clicked()
    {
        stop();
    }
    
    void MainWindow::on_station_list_tree_view_row_activated(const Gtk::TreeModel::Path& path,
                                                             Gtk::TreeViewColumn* column)
    {
        Gtk::TreeIter iter = stationListTreeView.get_model()->get_iter(path);
        int ID = (*iter)[stationListTreeViewColumns.IDColumn];
        play(stations[ID]);
    }
    
    void MainWindow::on_station_list_tree_view_selection_changed()
    {
        if (get_selected_station_iter())
        {
            set_station_widgets_sensitive(true); // Enable station widgets
        }
        else
        {
            set_station_widgets_sensitive(false); // Disable station widgets
        }
    }
    
    void MainWindow::on_playlist_history_scrolled_window_vadjustment_value_changed()
    {
        Glib::RefPtr<Gtk::Adjustment> pAdjustment = playlistHistoryScrolledWindow.get_vadjustment();
        
        if (pAdjustment->get_upper() - pAdjustment->get_page_size() != pAdjustment->get_value())
        {
            playlistHistoryTreeViewShallScrollToBottom = false;
        }
        else
        {
            playlistHistoryTreeViewShallScrollToBottom = true;
        }
    }
    
    void MainWindow::on_playlist_history_tree_view_size_allocate(Gtk::Allocation& allocation)
    {
        if (playlistHistoryTreeViewShallScrollToBottom)
        {
            Glib::RefPtr<Gtk::Adjustment> pAdjustment = playlistHistoryScrolledWindow.get_vadjustment();
            pAdjustment->set_value(pAdjustment->get_upper() - pAdjustment->get_page_size());
        }
    }
    
    void MainWindow::on_vlc_log_scrolled_window_vadjustment_value_changed()
    {
        Glib::RefPtr<Gtk::Adjustment> pAdjustment = vlcLogScrolledWindow.get_vadjustment();
        
        if (pAdjustment->get_upper() - pAdjustment->get_page_size() != pAdjustment->get_value())
        {
            vlcLogTreeViewShallScrollToBottom = false;
        }
        else
        {
            vlcLogTreeViewShallScrollToBottom = true;
        }
    }
    
    void MainWindow::on_vlc_log_tree_view_size_allocate(Gtk::Allocation& allocation)
    {
        if (vlcLogTreeViewShallScrollToBottom)
        {
            Glib::RefPtr<Gtk::Adjustment> pAdjustment = vlcLogScrolledWindow.get_vadjustment();
            pAdjustment->set_value(pAdjustment->get_upper() - pAdjustment->get_page_size());
        }
    }
    
    void MainWindow::on_media_meta_changed(libvlc_meta_t meta_type)
    {
        update_player_track_labels();
    }
    
    void MainWindow::on_player_encountered_error()
    {
        fail();
    }
    
    void MainWindow::on_vlc_log_line_available(Glib::ustring line)
    {
        // Get current time as string
        time_t rawTime = time(0);
        struct tm* timeInfo = localtime(&rawTime);
        char timeString[80];
        strftime (timeString, 80, "%H:%M:%S", timeInfo);
        
        // Insert new row in VLC log list store
        Gtk::TreeIter iter = pVlcLogListStore->append();
        Gtk::TreeModel::Row row = *iter;
        
        row[vlcLogTreeViewColumns.timeColumn] = timeString;
        row[vlcLogTreeViewColumns.messageColumn] = line;
    }
    
    
} // namespace jRadio
