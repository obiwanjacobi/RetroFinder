#include "List.h"

void List::DeclareContent(Theme* theme) {
    // Main container with vertical layout
    Layout().sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) };
    Layout().layoutDirection = CLAY_TOP_TO_BOTTOM;

    // Headers (if enabled)
    if (_showHeaders && _columns.size() > 0) {
        DrawHeaders(theme);
    }

    // Content area with items
    DrawRows(theme);
}

void List::DrawHeaders(Theme* theme) {
    CLAY_AUTO_ID({
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(_rowHeight) },
            .padding = { .left = 4, .right = 4, .top = 4, .bottom = 4 },
            .layoutDirection = CLAY_LEFT_TO_RIGHT
        },
        .backgroundColor = theme->GetBackgroundColor(),
        .border = { .color = theme->GetForegroundColor(), .width = { 0, 0, 3, 0 }}
    }) {
        for (size_t i = 0; i < _columns.size(); ++i) {
            const ListColumn& column = _columns[i];
            float colWidth = column.width > 0 ? column.width : 100.0f;

            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { .width = CLAY_SIZING_FIXED(colWidth), .height = CLAY_SIZING_GROW(0) },
                    .padding = { .left = 4, .right = 4, .top = 2, .bottom = 2 }
                }
            }) {
                CLAY_TEXT(column.headerName, CLAY_TEXT_CONFIG({
                    .userData = (void*)theme->GetFontArray(),
                    .textColor = theme->GetForegroundColor(),
                    .fontId = 0,
                    .fontSize = theme->GetFontSize(),
                    .letterSpacing = theme->GetFontSpacing(),
                }));
            }
        }
    }
}

void List::DrawRows(Theme* theme) {
    _rowClickContexts.resize(_items.size());

    for (size_t rowIndex = 0; rowIndex < _items.size(); ++rowIndex) {
        const ListItem& item = _items[rowIndex];
        bool isSelected = IsItemSelected(rowIndex);
        
        // Determine row background color
        Clay_Color rowBgColor = theme->GetBackgroundColor();
        Clay_Color rowFgColor = theme->GetForegroundColor();
        if (isSelected) {
            rowBgColor = theme->GetFocusColor();
            rowFgColor = theme->GetBackgroundColor();
        } else if (_useAlternateRowColor && rowIndex % 2 == 1) {
            rowBgColor = _alternateRowColor;
        }

        // Create unique ID for this row for pointer detection
        Clay_ElementId rowId = CLAY_IDI("list_row", static_cast<uint32_t>(rowIndex));

        CLAY(rowId, {
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(_rowHeight) },
                .padding = { .left = 0, .right = 0, .top = 0, .bottom = 0 },
                .layoutDirection = CLAY_LEFT_TO_RIGHT
            },
            .backgroundColor = rowBgColor
        }) {
            _rowClickContexts[rowIndex] = { .list = this, .rowIndex = rowIndex };
            Clay_OnHover(OnRowHoverCallback, &_rowClickContexts[rowIndex]);

            // Draw cells for each column
            for (size_t colIndex = 0; colIndex < _columns.size(); ++colIndex) {
                const ListColumn& column = _columns[colIndex];
                float colWidth = column.width > 0 ? column.width : 100.0f;
                
                Clay_String cellText = item.GetValue(colIndex);

                CLAY_AUTO_ID({
                    .layout = {
                        .sizing = { .width = CLAY_SIZING_FIXED(colWidth), .height = CLAY_SIZING_GROW(0) },
                    }
                }) {
                    CLAY_TEXT(cellText, CLAY_TEXT_CONFIG({
                        .userData = (void*)theme->GetFontArray(),
                        .textColor = rowFgColor,
                        .fontId = 0,
                        .fontSize = theme->GetFontSize(),
                        .letterSpacing = theme->GetFontSpacing(),
                    }));
                }
            }
        }
    }
}
