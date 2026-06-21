#include "Window.h"
#include "project.h"
#include <stdlib.h>

void ErrorHandler(Clay_ErrorData errorText)
{
    //Window* window = static_cast<Window*>(errorText.userData);
    TraceLog(LOG_ERROR, "Clay Error: %s", 
        ClayStringToCString(ClayStringToSlice(errorText.errorText)));
}

Window::Window(ConfigFlags configFlags) : _screen(CLAY_ID("screen")) {
    // setup memory arena
    uint32_t memSize = Clay_MinMemorySize();
    void* mem = malloc(memSize);
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(memSize, mem);

    Clay_Raylib_Initialize(1024, 768, "Clay/Raylib Test", configFlags);
    Clay_Initialize(arena,  Clay_Dimensions{1024, 768}, Clay_ErrorHandler{ ErrorHandler, this });
    Clay_SetMeasureTextFunction(Raylib_MeasureText, nullptr);
}

void Window::Declare(Theme* theme)
{
    _screen.Layout() = Clay_LayoutConfig { 
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
        .padding = { .left = 12, .right = 12, .top = 12, .bottom = 12 },
        .childAlignment = { .x = CLAY_ALIGN_X_CENTER },
        .layoutDirection = CLAY_TOP_TO_BOTTOM
    };
    _screen.SetBorder({ .color = theme->GetForegroundColor(), .width = CLAY_BORDER_OUTSIDE(6) });
    _screen.Declare(_theme);
}

void Window::Render()
{
    if (!_theme) {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText("ERROR: No Theme set to render.", 10, 10, 60, RED);
        EndDrawing();
        return;
    }

    Clay_SetLayoutDimensions({ (float)GetScreenWidth(), (float)GetScreenHeight() });
    auto mouse = GetMousePosition();
    Clay_SetPointerState({ mouse.x, mouse.y }, IsMouseButtonDown(MOUSE_LEFT_BUTTON));
    Clay_UpdateScrollContainers(true, 
        RaylibVector2ToClayVector2Scale(GetMouseWheelMoveV(), 1, 3), GetFrameTime());

    Clay_BeginLayout();
    Declare(_theme);
    auto commands = Clay_EndLayout(GetFrameTime());

    BeginDrawing();
    ClearBackground(ClayColorToRaylibColor(_theme->GetBackgroundColor()));
    Clay_Raylib_Render(commands, _theme->GetFontArray());
    EndDrawing();
}
