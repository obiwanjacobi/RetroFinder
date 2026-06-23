#pragma once
#include "project.h"
#include "ContainerControl.h"

class Panel : public ContainerControl {
public:
    Panel() : Panel(CLAY_ID("panel")) {}
    explicit Panel(Clay_ElementId id) : ContainerControl(id) {}
    ~Panel() = default;

    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;
    Panel(Panel&&) = delete;
    Panel& operator=(Panel&&) = delete;
};
