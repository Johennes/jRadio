//////////////////////////////////////////////////////////////////////////////
// StationListColumnRecord.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#include "StationListColumnRecord.h"


namespace jRadio
{
    
    
    // Constructor & destructor
    
    StationListColumnRecord::StationListColumnRecord()
    {
        // Add columns
        add (IDColumn);
        add (titleColumn);
        add (genreColumn);
    }
    
    StationListColumnRecord::~StationListColumnRecord()
    {
    }
    
    
} // namespace jRadio
