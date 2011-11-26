//////////////////////////////////////////////////////////////////////////////
// VLCLogColumnRecord.cpp
//
// Copyright 2011 Johannes Marbach. All rights reserved.
// See the LICENSE file for details.


#include "VLCLogColumnRecord.h"


namespace jRadio
{
    
    
    // Constructor & destructor
    
    VLCLogColumnRecord::VLCLogColumnRecord()
    {
        // Add columns
        add (timeColumn);
        add (messageColumn);
    }
    
    VLCLogColumnRecord::~VLCLogColumnRecord()
    {
    }
    
    
} // namespace jRadio
