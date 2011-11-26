//////////////////////////////////////////////////////////////////////////////
// VLC.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#include "VLC.h"

#include "FileUtil.h"

#include <sstream>
#include <stdexcept>

#include <sys/stat.h>

#include <fcntl.h>


namespace jRadio
{
    
    
    // Constructor & destructor
    
    VLC::VLC(std::string logFile, int logLevel) :
        pMedia(0),
        pMediaList(0),
        logFD(-1),
        pLogChannel(0)
    {
        // Convert log level to string
        std::ostringstream sStream;
        sStream << logLevel;
        const char* logLevelString = sStream.str().c_str();
        
        // Create VLC instance
        const char* const argv[] = {
            "--quiet", "--file-logging",
            "--logfile", logFile.c_str(),
            "--log-verbose", logLevelString
        };  
        pInstance = libvlc_new(6, argv);
        
        // Create players
        pPlayer = libvlc_media_player_new(pInstance);
        pListPlayer = libvlc_media_player_new(pInstance);
        pMediaListPlayer = libvlc_media_list_player_new(pInstance);
        libvlc_media_list_player_set_media_player(pMediaListPlayer, pListPlayer);
        
        // Setup event handlers
        libvlc_event_manager_t* pPlayerEventManager = libvlc_media_player_event_manager(pPlayer);
        libvlc_event_attach(pPlayerEventManager, libvlc_MediaPlayerEncounteredError,
            &on_media_player_encountered_error_static, (void*)this);
        libvlc_event_attach(pPlayerEventManager, libvlc_MediaPlayerEndReached,
            &on_media_player_end_reached_static, (void*)this);
        libvlc_event_attach(pPlayerEventManager, libvlc_MediaPlayerPlaying,
            &on_media_player_playing_static, (void*)this);
        
        libvlc_event_manager_t* pListPlayerEventManager = libvlc_media_player_event_manager(pListPlayer);
        libvlc_event_attach(pListPlayerEventManager, libvlc_MediaPlayerEncounteredError,
            &on_media_player_encountered_error_static, (void*)this);
        libvlc_event_attach(pListPlayerEventManager, libvlc_MediaPlayerEndReached,
            &on_media_player_end_reached_static, (void*)this);
        libvlc_event_attach(pListPlayerEventManager, libvlc_MediaPlayerPlaying,
            &on_media_player_playing_static, (void*)this);
        
        // Setup log file monitoring
        logFilePath = logFile;
        pLogFile = Gio::File::create_for_path(logFile);
        pLogFileMonitor = pLogFile->monitor_file();
        pLogFileMonitor->signal_changed().connect(sigc::mem_fun(*this, &VLC::on_log_file_changed));
    }
    
    VLC::~VLC()
    {
        shutdown();
    }
    
    
    // Playback methods
    
    void VLC::play(std::string MRL)
    {
        // Stop player
        if (libvlc_media_player_is_playing (pPlayer))
        {
            libvlc_media_player_stop(pPlayer);
        }
        else if (pMediaList && libvlc_media_list_player_is_playing (pMediaListPlayer))
        {
            libvlc_media_list_player_stop(pMediaListPlayer);
        }
        
        // Reset media
        if (pMedia)
        {
            libvlc_media_release(pMedia);
            pMedia = 0;
        }
        
        // Reset media list
        if (pMediaList)
        {
            libvlc_media_list_release(pMediaList);
            pMediaList = 0;
        }
        
        // Create new media with MRL
        pMedia = libvlc_media_new_location(pInstance, MRL.c_str());
        
        // Setup media event handlers
        libvlc_event_manager_t* pEventManager = libvlc_media_event_manager(pMedia);
        libvlc_event_attach(pEventManager, libvlc_MediaMetaChanged,
            &on_media_meta_changed_static, (void*)this);
        
        // Play media
        libvlc_media_player_set_media(pPlayer, pMedia);
        libvlc_media_player_play(pPlayer);
    }
    
    void VLC::resume()
    {
        if (libvlc_media_player_will_play (pPlayer))
        {
            libvlc_media_player_play(pPlayer);
        }
        else if (libvlc_media_player_will_play (pListPlayer))
        {
            libvlc_media_player_play(pListPlayer);
        }
    }
    
    void VLC::pause()
    {
        if (libvlc_media_player_is_playing (pPlayer))
        {
            libvlc_media_player_pause(pPlayer);
        }
        else if (pMediaList && libvlc_media_list_player_is_playing (pMediaListPlayer))
        {
            libvlc_media_list_player_pause(pMediaListPlayer);
        }
    }
    
    void VLC::stop()
    {
        if (libvlc_media_player_is_playing (pPlayer))
        {
            libvlc_media_player_stop(pPlayer);
        }
        else if (pMediaList && libvlc_media_list_player_is_playing (pMediaListPlayer))
        {
            libvlc_media_list_player_stop(pMediaListPlayer);
        }
    }
    
    
    // Volume methods
    
    int VLC::get_volume() const
    {
        if (pPlayer && pListPlayer)
        {
            return libvlc_audio_get_volume (pPlayer);
        }
        else
        {
            return -1;
        }
    }
    
    void VLC::set_volume(int value)
    {
        if (pPlayer && pListPlayer)
        {
            libvlc_audio_set_volume (pPlayer, value);
            libvlc_audio_set_volume (pListPlayer, value);
        }
    }
    
    
    // Metadata methods
    
    std::string VLC::get_meta(libvlc_meta_t type)
    {
        if (pMedia)
        {
            char* meta = libvlc_media_get_meta (pMedia, type);
            return (meta) ? std::string(meta) : "";
        }
        else
        {
            return "";
        }
    }
    
    std::string VLC::get_meta_artist()
    {
        return get_meta(libvlc_meta_Artist);
    }
    
    std::string VLC::get_meta_title()
    {
        return get_meta(libvlc_meta_Title);
    }
    
    std::string VLC::get_meta_now_playing()
    {
        return get_meta(libvlc_meta_NowPlaying);
    }
    
    
    // Playback state methods
    
    bool VLC::is_playing()
    {
        return (bool)libvlc_media_player_is_playing(pPlayer) || (bool)libvlc_media_player_is_playing(pListPlayer);
    }
    
    bool VLC::can_play()
    {
        return (bool)libvlc_media_player_will_play(pPlayer) || (bool)libvlc_media_player_will_play(pListPlayer);
    }
    
    
    // Miscellaneous methods
    void VLC::shutdown()
    {
        if (pInstance)
        {
            libvlc_release(pInstance);
            pInstance = 0;
        }
        
        if (pPlayer)
        {
            libvlc_media_player_release(pPlayer);
            pPlayer = 0;
        }
        
        if (pListPlayer)
        {
            libvlc_media_player_release(pListPlayer);
            pListPlayer = 0;
        }
        
        if (pMediaListPlayer)
        {
            libvlc_media_list_player_release(pMediaListPlayer);
            pMediaListPlayer = 0;
        }
        
        if (pMedia)
        {
            libvlc_media_release(pMedia);
            pMedia = 0;
        }
        
        if (pMediaList)
        {
            libvlc_media_list_release(pMediaList);
            pMediaList = 0;
        }
        
        pLogFile.reset();
        pLogFileMonitor.reset();
        
        if (logFD >= 0)
        {
            pLogChannel->close();
            pLogChannel.reset();
            close(logFD);
            logFD = -1;
        }
        
        logFilePath = "";
    }
    
    
    // Event handlers
    
    void VLC::on_media_meta_changed(const libvlc_event_t* event, void* data)
    {
        media_meta_changed_signal.emit(event->u.media_meta_changed.meta_type);
    }
    
    void VLC::on_media_player_encountered_error(const libvlc_event_t* event, void* data)
    {
        player_encountered_error_signal.emit();
    }
    
    void VLC::on_media_player_end_reached(const libvlc_event_t* event, void* data)
    {
        // Should only happen if a playlist was loaded
        
        if (! pMediaList)
        {
            pMediaList = libvlc_media_subitems(pMedia); // Try to get media list
            
            // Reset media pointer
            libvlc_media_release(pMedia);
            pMedia = 0;
            
            if (pMediaList)
            {
                // Get first media
                libvlc_media_list_lock(pMediaList);
                pMedia = libvlc_media_list_item_at_index(pMediaList, 0); // Increases media retain count
                libvlc_media_list_unlock(pMediaList);
                
                // Setup event handlers
                libvlc_event_manager_t* pEventManager = libvlc_media_event_manager(pMedia);
                libvlc_event_attach(pEventManager, libvlc_MediaMetaChanged,
                    &on_media_meta_changed_static, (void*)this);
                
                // Play media list
                libvlc_media_list_player_set_media_list(pMediaListPlayer, pMediaList);
                libvlc_media_list_player_play(pMediaListPlayer);
            }
        }
    }
    
    void VLC::on_media_player_playing(const libvlc_event_t* event, void* data)
    {
        player_starts_playing_signal.emit();
    }
    
    void VLC::on_log_file_changed(const Glib::RefPtr<Gio::File>& pFile,
                                  const Glib::RefPtr<Gio::File>& pOtherFile,
                                  Gio::FileMonitorEvent eventType)
    {
        static gint64 pos = 0; // Remember position of last read operation
        
        if (eventType == Gio::FILE_MONITOR_EVENT_CHANGED)
        {
            if (logFD < 0)
            {
                // Open log file
                logFD = open(logFilePath.c_str(), O_RDONLY | O_NONBLOCK);
                if (logFD == -1)
                {
                    throw std::runtime_error("Failed to open VLC log file for reading.");
                }
                
                // Create IO channel
                pLogChannel = Glib::IOChannel::create_from_fd(logFD);
            }
            
            // Define polling time
            struct timeval pollInterval;
            pollInterval.tv_sec = 0.1;
            pollInterval.tv_usec = 0;
            
            // Setup file descriptor set
            fd_set FDs;
            FD_ZERO (&FDs);
            FD_SET (logFD, &FDs);
            
            while (true)
            {
                int ret = select (logFD + 1, &FDs, NULL, NULL, &pollInterval);
                
                if (ret < 0)
                {
                    throw std::runtime_error("Failed to select VLC log file.");
                }
                else if (FD_ISSET (logFD, &FDs))
                {
                    pLogChannel->seek(pos); // Jump to last read position
                    
                    // Read new lines
                    Glib::ustring line;
                    while (pLogChannel->read_line(line) != Glib::IO_STATUS_EOF)
                    {
                        pos += line.length(); // Update last read position
                        
                        log_line_available_signal.emit(line.substr(0, line.length() - 1)); // Notify about new log line
                    }
                    
                    break;
                }
            }
        }
    }
    
    
    // Static event handlers
    
    void VLC::on_media_meta_changed_static(const libvlc_event_t* event, void* data)
    {
        ((VLC*)data)->on_media_meta_changed(event);
    }
    
    void VLC::on_media_player_encountered_error_static(const libvlc_event_t* event, void* data)
    {
        ((VLC*)data)->on_media_player_encountered_error(event);
    }
    
    void VLC::on_media_player_end_reached_static(const libvlc_event_t* event, void* data)
    {
        ((VLC*)data)->on_media_player_end_reached(event);
    }
    
    void VLC::on_media_player_playing_static(const libvlc_event_t* event, void* data)
    {
        ((VLC*)data)->on_media_player_playing(event);
    }
    
    
} // namespace jRadio
