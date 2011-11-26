//////////////////////////////////////////////////////////////////////////////
// VLC.h
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#ifndef H_VLC
#define H_VLC


#include <string>

#include <glibmm/iochannel.h>

#include <giomm/file.h>
#include <giomm/filemonitor.h>

#include <sigc++/signal.h>

#include <vlc/vlc.h>


namespace jRadio
{
    
    
    class VLC
    {
        public:
            VLC(std::string logFile, int logLevel = 2);
            ~VLC();
            
            void play(std::string MRL);
            void resume();
            void pause();
            void stop();
            
            int get_volume() const;
            void set_volume(int value);
            
            std::string get_meta(libvlc_meta_t type);
            std::string get_meta_artist();
            std::string get_meta_title();
            std::string get_meta_now_playing();
            
            bool is_playing();
            bool can_play();
            
            void shutdown();
            
            sigc::signal<void, libvlc_meta_t> media_meta_changed_signal;
            sigc::signal<void> player_encountered_error_signal;
            sigc::signal<void> player_starts_playing_signal;
            sigc::signal<void, Glib::ustring> log_line_available_signal;
        
        protected:
            libvlc_instance_t* pInstance;
            libvlc_media_player_t* pPlayer;
            libvlc_media_player_t* pListPlayer;
            libvlc_media_list_player_t* pMediaListPlayer;
            libvlc_media_t* pMedia;
            libvlc_media_list_t* pMediaList;
            
            std::string logFilePath;
            int logFD;
            Glib::RefPtr<Glib::IOChannel> pLogChannel;
            Glib::RefPtr<Gio::File> pLogFile;
            Glib::RefPtr<Gio::FileMonitor> pLogFileMonitor;
            
            void on_media_meta_changed(const libvlc_event_t* event, void* data = 0);
            void on_media_player_encountered_error(const libvlc_event_t* event, void* data = 0);
            void on_media_player_end_reached(const libvlc_event_t* event, void* data = 0);
            void on_media_player_playing(const libvlc_event_t* event, void* data = 0);
            void on_log_file_changed(const Glib::RefPtr<Gio::File>& pFile,
                                     const Glib::RefPtr<Gio::File>& pOtherFile,
                                     Gio::FileMonitorEvent eventType);
            
            static void on_media_meta_changed_static(const libvlc_event_t* event, void* data);
            static void on_media_player_encountered_error_static(const libvlc_event_t* event, void* data);
            static void on_media_player_end_reached_static(const libvlc_event_t* event, void* data);
            static void on_media_player_playing_static(const libvlc_event_t* event, void* data);
    };
    
    
} // namespace jRadio


#endif // H_VLC
