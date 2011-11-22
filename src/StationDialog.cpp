//
//
//      StationDialog.cpp
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


#include "StationDialog.h"

#include "CURLLoader.h"
#include "FileUtil.h"
#include "Settings.h"

#include <iostream>

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>


namespace jRadio
{
    
    
    // Constructor & destructor
    
    StationDialog::StationDialog(Gtk::Window& parent, Station& station) :
        titleLabel("Title"),
        genreLabel("Genre"),
        MRLLabel("MRL"),
        rStation(station)
    {
        set_transient_for(parent); // Setup parent window
        
        // Add action buttons
        add_button (Gtk::Stock::OK, Gtk::RESPONSE_OK);
        add_button (Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        
        // Insert main grid
        get_vbox()->pack_start(mainGrid);
        mainGrid.set_row_spacing(10);
        mainGrid.set_column_spacing(10);
        mainGrid.set_margin_top(20);
        mainGrid.set_margin_right(20);
        mainGrid.set_margin_bottom(20);
        mainGrid.set_margin_left(20);
        mainGrid.set_valign(Gtk::ALIGN_CENTER);
        
        // Insert child widgets
        iconButton.add(icon);
        
        mainGrid.attach(iconButton, 0, 0, 2, 1);
        mainGrid.attach(titleLabel, 0, 1, 1, 1);
        mainGrid.attach(titleEntry, 1, 1, 1, 1);
        mainGrid.attach(genreLabel, 0, 2, 1, 1);
        mainGrid.attach(genreEntry, 1, 2, 1, 1);
        mainGrid.attach(MRLLabel, 0, 3, 1, 1);
        mainGrid.attach(MRLEntry, 1, 3, 1, 1);
        
        // Setup expansion & alignment rules
        iconButton.set_hexpand(false);
        iconButton.set_halign(Gtk::ALIGN_CENTER);
        titleEntry.set_hexpand(true);
        genreEntry.set_hexpand(true);
        MRLEntry.set_hexpand(true);
        
        // Connect event handlers
        iconButton.signal_clicked().connect(sigc::mem_fun(*this, &StationDialog::on_icon_button_clicked));
        
        // Insert station details
        titleEntry.set_text(rStation.title);
        genreEntry.set_text(rStation.genre);
        MRLEntry.set_text(rStation.MRL);
        
        update_icon();
        
        show_all_children();
    }
    
    StationDialog::~StationDialog()
    {
    }
    
    
    // Data retrieval methods
    
    std::string StationDialog::get_station_title()
    {
        return titleEntry.get_text();
    }
    
    std::string StationDialog::get_station_genre()
    {
        return genreEntry.get_text();
    }
    
    std::string StationDialog::get_station_mrl()
    {
        return MRLEntry.get_text();
    }
    
    
    // Miscellaneous methods
    
    void StationDialog::update_icon()
    {
        try
        {
            Glib::RefPtr<Gdk::Pixbuf> pPixbuf = Gdk::Pixbuf::create_from_file(rStation.iconPath, 128, 128);
            icon.set(pPixbuf);
        }
        catch (...)
        {
            FileUtil::remove_file(rStation.iconPath);
            rStation.iconPath = "";
            icon.set(Settings::default_icon_path());
        }
    }
    
    
    // Event handlers
    
    void StationDialog::on_icon_button_clicked()
    {
        // Create icon source type chooser dialog
        Gtk::MessageDialog sourceDialog(*this, "From where do you want to load this station's icon?", true,
            Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE);
        
        // Add action buttons
        int responseFile = 1, responseURL = 2;
        
        Gtk::Image fileImage(Gtk::Stock::OPEN, Gtk::ICON_SIZE_BUTTON);
        Gtk::Button fileButton("From file");
        fileButton.set_image(fileImage);
        sourceDialog.add_action_widget(fileButton, responseFile);
        
        Gtk::Image networkImage(Gtk::Stock::NETWORK, Gtk::ICON_SIZE_BUTTON);
        Gtk::Button networkButton("From URL");
        networkButton.set_image(networkImage);
        sourceDialog.add_action_widget(networkButton, responseURL);
        
        sourceDialog.add_button (Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        
        sourceDialog.show_all_children();
        
        // Run dialog
        int response = sourceDialog.run();
        sourceDialog.hide();
        
        if (response == responseFile)
        {
            // Load file chooser dialog
            Gtk::FileChooserDialog dialog(*this, "Choose an image file", Gtk::FILE_CHOOSER_ACTION_OPEN);
            dialog.add_button (Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
            dialog.add_button (Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
            
            if (dialog.run() == Gtk::RESPONSE_OK)
            {
                std::string* pError = 0;
                
                // Try to load the selected file
                
                try
                {
                    Glib::RefPtr<Gdk::Pixbuf> pPixbuf = Gdk::Pixbuf::create_from_file(dialog.get_filename());
                }
                catch (Glib::FileError error)
                {
                    pError = new std::string("Could not access the selected file.");
                }
                catch (Gdk::PixbufError error)
                {
                    pError = new std::string("This does not seem to be a supported image format.");
                }
                
                if (pError)
                {
                    // Alert error
                    Gtk::MessageDialog errorDialog(*this, *pError, false, Gtk::MESSAGE_ERROR);
                    errorDialog.run();
                    delete pError;
                }
                else
                {
                    // Copy file to internal data directory
                    std::string filename = dialog.get_filename();
                    std::string internalFilename = FileUtil::get_internal_filename(rStation.ID, filename);
                    
                    FileUtil::remove_file(rStation.iconPath); // Neccessary due to potentially distinct file extension
                    FileUtil::copy_file(filename, internalFilename);
                    rStation.iconPath = internalFilename;
                    
                    update_icon();
                }

            }
        }
        else if (response == responseURL)
        {
            // Load URL input dialog
            Gtk::Dialog dialog("Enter the image's URL", *this);
            
            Gtk::Entry URLEntry;
            dialog.get_vbox()->pack_start(URLEntry);
            
            dialog.add_button (Gtk::Stock::OK, Gtk::RESPONSE_OK);
            dialog.add_button (Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
            
            dialog.show_all_children();
            
            if (dialog.run() == Gtk::RESPONSE_OK)
            {
                try
                {
                    CURLLoader::download(URLEntry.get_text(), Settings::user_data_dir() + std::string("/tmp"));
                }
                catch (const std::runtime_error& error)
                {
                    // Display error message dialog
                    Gtk::MessageDialog errorDialog(*this,
                        "Download failed. Please check the URL or your network connection.", true,
                        Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE);
                    errorDialog.run();
                    
                    FileUtil::remove_file(Settings::user_data_dir() + std::string("/tmp")); // Remove tmp file
                    
                    return;
                }
                
                // Move file to separate resource in internal data directory
                std::string internalFilename = FileUtil::get_internal_filename(rStation.ID, "");
                
                FileUtil::remove_file(rStation.iconPath); // Neccessary due to potentially distinct file extension
                FileUtil::copy_file(Settings::user_data_dir() + std::string("/tmp"), internalFilename);
                FileUtil::remove_file(Settings::user_data_dir() + std::string("/tmp")); // Remove tmp file
                rStation.iconPath = internalFilename;
                
                update_icon();
            }
        }
    }
    
    
} // namespace jRadio
