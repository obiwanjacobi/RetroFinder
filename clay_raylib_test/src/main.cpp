#include "raylib.h"
#include "clay.h"
#include "Window.h"
#include "Label.h"
#include "Theme.h"
#include "ThemeColors.h"

Window* g_window = nullptr;
//RenderContext* g_renderContext = nullptr;
Theme* g_theme = nullptr;

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 768;

    g_window = new Window(ConfigFlags::FLAG_WINDOW_RESIZABLE);
    g_window->SetSize(screenWidth, screenHeight);
    g_window->SetMinSize(450, 450);
    g_window->SetTitle("Raylib/Clay Test");

    SetTargetFPS(60);

    //g_renderContext = new RenderContext();
    g_theme = new Theme();
    g_theme->SetBackgroundColor(CLAY_COLOR_WHITE);
    g_theme->SetForegroundColor(CLAY_COLOR_BLACK);
    if (!g_theme->LoadFont("./ChicagoFLF.ttf", 16)) {
        TraceLog(LOG_ERROR, "Theme: Failed to load font.");
    }
    g_theme->SetFontSize(48);
    g_window->SetTheme(g_theme);

    Label lbl;
    lbl.SetText(CLAY_STRING("raylib template integrated (PowerShell build)"));
    g_window->AddControl(&lbl);

    while (!g_window->ShouldClose()) {
        g_window->Render();
    }
    
    CloseWindow();
    delete g_theme;
    //delete g_renderContext;
    delete g_window;
    return 0;
}
