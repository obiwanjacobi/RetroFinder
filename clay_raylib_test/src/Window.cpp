#include "Window.h"
#include "clay_renderer_raylib.h"
#include <cstring>

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

    _screen.Layout() = Clay_LayoutConfig { 
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
        .padding = { 16, 16, 16, 16 }, 
        .childGap = 16,
        .layoutDirection = CLAY_TOP_TO_BOTTOM
    };
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
    //Clay_UpdateScrollContainers(true, GetMouseWheelMoveV(), GetFrameTime());

    Clay_BeginLayout();
    _screen.Declare(_theme);
    auto commands = Clay_EndLayout(GetFrameTime());

    BeginDrawing();
    ClearBackground(ClayColorToRaylibColor(_theme->GetBackgroundColor()));
    Clay_Raylib_Render(commands, _theme->GetFontArray());
    EndDrawing();
}
