//////////////////////////////////////////////////////////////////////////////
// main.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#include "MainWindow.h"

#include <iostream>
#include <locale>

#include <gtkmm/main.h>


using namespace jRadio;

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
     
    Gtk::Main gtk(argc, argv);
    MainWindow window;
    Gtk::Main::run(window);

    return EXIT_SUCCESS;
}
