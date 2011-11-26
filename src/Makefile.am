##############################################################################
## Makefile.am
##
## Copyright 2011 Johannes Marbach. All rights reserved.
## See the LICENSE file for details.

bin_PROGRAMS = jRadio

jRadio_SOURCES = main.cpp \
                 AboutDialog.cpp AboutDialog.h \
                 CURLLoader.cpp CURLLoader.h \
                 FileUtil.cpp FileUtil.h \
                 MainWindow.cpp MainWindow.h \
                 PlaylistHistoryColumnRecord.cpp PlaylistHistoryColumnRecord.h \
                 Settings.cpp Settings.h \
                 Station.cpp Station.h \
                 StationDialog.cpp StationDialog.h \
                 StationListColumnRecord.cpp StationListColumnRecord.h \
                 StationMap.cpp StationMap.h \
                 VLC.cpp VLC.h \
                 VLCLogColumnRecord.cpp VLCLogColumnRecord.h

LIBS = $(gtkmm_LIBS) $(libcurl_LIBS) $(libvlc_LIBS) $(libxml_LIBS)
AM_CXXFLAGS = $(gtkmm_CFLAGS) $(libcurl_CFLAGS) $(libvlc_CFLAGS) $(libxml_CFLAGS) \
              -DDATADIR='"$(datadir)/jRadio"'
