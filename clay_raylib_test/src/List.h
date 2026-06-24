#pragma once

#include "project.h"
#include "Control.h"
#include "ListColumn.h"
#include "ListItem.h"

#include <vector>
#include <functional>

class List : public Control {
public:
    using SelectionHandler = std::function<void(size_t selectedIndex)>;

    List() : List(CLAY_ID("list")) {}
    explicit List(Clay_ElementId id) : Control(id) {}
    ~List() = default;

    List(const List&) = delete;
    List& operator=(const List&) = delete;
    List(List&&) = delete;
    List& operator=(List&&) = delete;

    // Column management
    void AddColumn(const ListColumn& column) {
        _columns.push_back(column);
    }
    
    void ClearColumns() {
        _columns.clear();
    }
    
    size_t GetColumnCount() const {
        return _columns.size();
    }
    
    const ListColumn& GetColumn(size_t index) const {
        return _columns[index];
    }

    // Item management
    void AddItem(const ListItem& item) {
        _items.push_back(item);
    }
    
    void AddItem(const std::vector<Clay_String>& values) {
        _items.emplace_back(values);
    }
    
    void ClearItems() {
        _items.clear();
    }
    
    size_t GetItemCount() const {
        return _items.size();
    }
    
    const ListItem& GetItem(size_t index) const {
        return _items[index];
    }
    
    ListItem& GetItem(size_t index) {
        return _items[index];
    }

    // Selection
    void SetSelectedIndex(int index) {
        _selectedIndex = index;
    }
    
    int GetSelectedIndex() const {
        return _selectedIndex;
    }
    
    bool IsItemSelected(size_t index) const {
        return _selectedIndex == static_cast<int>(index);
    }
    
    void SetOnSelectionChanged(SelectionHandler handler) {
        _onSelectionChanged = std::move(handler);
    }
    
    void ClearOnSelectionChanged() {
        _onSelectionChanged = nullptr;
    }

    // Styling
    void SetShowHeaders(bool show) { _showHeaders = show; }
    bool GetShowHeaders() const { return _showHeaders; }
    
    void SetRowHeight(float height) { _rowHeight = height; }
    float GetRowHeight() const { return _rowHeight; }
        
    void SetAlternateRowColor(Clay_Color color) { 
        _alternateRowColor = color;
        _useAlternateRowColor = true;
    }
    void DisableAlternateRowColor() { _useAlternateRowColor = false; }

protected:
    void DeclareContent(Theme* theme) override;

private:
    struct RowClickContext {
        List* list = nullptr;
        size_t rowIndex = 0;
    };

    static void OnRowHoverCallback(Clay_ElementId, Clay_PointerData pointerData, void* userData) {
        if (pointerData.state != CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
            return;
        }

        RowClickContext* context = static_cast<RowClickContext*>(userData);
        if (!context || !context->list) {
            return;
        }

        List* list = context->list;
        const int newIndex = static_cast<int>(context->rowIndex);
        if (list->_selectedIndex == newIndex) {
            return;
        }

        list->_selectedIndex = newIndex;
        if (list->_onSelectionChanged) {
            list->_onSelectionChanged(context->rowIndex);
        }
    }

    void DrawHeaders(Theme* theme);
    void DrawRows(Theme* theme);

    std::vector<ListColumn> _columns;
    std::vector<ListItem> _items;
    std::vector<RowClickContext> _rowClickContexts;
    
    int _selectedIndex = -1;
    SelectionHandler _onSelectionChanged;
    
    bool _showHeaders = true;
    float _rowHeight = 24.0f;
    
    Clay_Color _alternateRowColor = {45, 45, 45, 255};
    bool _useAlternateRowColor = false;
};
