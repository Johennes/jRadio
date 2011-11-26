//////////////////////////////////////////////////////////////////////////////
// PlaylistHistoryColumnRecord.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#include "PlaylistHistoryColumnRecord.h"


namespace jRadio
{
    
    
    // Constructor & destructor
    
    PlaylistHistoryColumnRecord::PlaylistHistoryColumnRecord()
    {
        // Add columns
        add (timeColumn);
        add (stationColumn);
        add (metaColumn);
    }
    
    PlaylistHistoryColumnRecord::~PlaylistHistoryColumnRecord()
    {
    }
    
    
} // namespace jRadio
