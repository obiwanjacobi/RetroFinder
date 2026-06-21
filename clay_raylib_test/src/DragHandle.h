#pragma once
#include "project.h"
#include <functional>

class DragHandle {
public:
    using DragCallback = std::function<void(Vector2 delta)>;
    using CanStartDragPredicate = std::function<bool()>;

    DragHandle() = default;
    ~DragHandle() = default;

    DragHandle(const DragHandle&) = delete;
    DragHandle& operator=(const DragHandle&) = delete;
    DragHandle(DragHandle&&) = delete;
    DragHandle& operator=(DragHandle&&) = delete;

    // Set callback invoked each frame with the mouse delta while dragging
    void SetOnDrag(DragCallback callback) { _onDrag = std::move(callback); }
    void ClearOnDrag() { _onDrag = nullptr; }

    // Set predicate to determine if drag can start
    // If not set, dragging can start whenever the left mouse button is pressed
    void SetCanStartDragPredicate(CanStartDragPredicate predicate) { _canStartDragPredicate = std::move(predicate); }
    void ClearCanStartDragPredicate() { _canStartDragPredicate = nullptr; }

    // Update must be called once per frame with the current mouse screen position
    void Update(Vector2 mouseScreenPos);

    bool IsDragging() const { return _isDragging; }

private:
    DragCallback _onDrag;
    CanStartDragPredicate _canStartDragPredicate;
    bool _isDragging = false;
    Vector2 _lastMouseScreenPos = { 0, 0 };
};
