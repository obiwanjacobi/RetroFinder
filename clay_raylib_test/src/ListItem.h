#pragma once

#include "project.h"
#include <vector>
#include <string>

struct ListItem {
    std::vector<Clay_String> values;
    
    ListItem() = default;
    explicit ListItem(const std::vector<Clay_String>& vals) : values(vals) {}
    
    void AddValue(Clay_String value) {
        values.push_back(value);
    }
    
    size_t GetValueCount() const {
        return values.size();
    }
    
    const Clay_String& GetValue(size_t index) const {
        if (index < values.size()) {
            return values[index];
        }
        static Clay_String empty = CLAY_STRING("");
        return empty;
    }
};
