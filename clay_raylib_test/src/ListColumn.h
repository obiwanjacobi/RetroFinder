#pragma once

#include "project.h"

struct ListColumn {
    Clay_String headerName;
    float width = 100.0f;  // Column width in pixels; 0 means auto-fit
    
    ListColumn() = default;
    ListColumn(float w) 
        : headerName(CLAY_STRING("")), width(w) {}
};
