#include "DragHandle.h"

void DragHandle::Update(Vector2 mouseScreenPos)
{
    // Check if we should start dragging
    if (!_isDragging && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Start dragging if no predicate, or predicate returns true
        bool canStart = _canStartDragPredicate ? _canStartDragPredicate() : true;
        if (canStart) {
            _isDragging = true;
            _lastMouseScreenPos = mouseScreenPos;
        }
    }

    // Stop dragging if mouse button released
    if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        _isDragging = false;
    }

    // Fire callback if dragging and we have a handler
    if (_isDragging && _onDrag) {
        Vector2 delta = {
            mouseScreenPos.x - _lastMouseScreenPos.x,
            mouseScreenPos.y - _lastMouseScreenPos.y
        };
        _lastMouseScreenPos = mouseScreenPos;
        _onDrag(delta);
    }
}
