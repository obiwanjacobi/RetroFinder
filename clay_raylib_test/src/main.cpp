#include "project.h"
#include "MacWindow.h"
#include "Label.h"
#include "List.h"
#include "Theme.h"
#include "ThemeColors.h"

MacWindow* g_window = nullptr;
Theme* g_theme = nullptr;

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 768;

    g_window = new MacWindow();
    g_window->SetSize(screenWidth, screenHeight);
    g_window->SetMinSize(450, 450);
    g_window->SetTitle("Raylib/Clay Test");

    SetTargetFPS(60);

    g_theme = new Theme();
    if (!g_theme->LoadFont("./ChicagoFLF.ttf", 48)) {
        TraceLog(LOG_ERROR, "Theme: Failed to load font.");
    }
    g_window->SetTheme(g_theme);

    // auto lblTxt = CLAY_STRING("raylib-clay test application - scroll panel test - long string for horizontal scroll ...");
    // for (int i = 0; i < 20; i++)
    // {
    //     Label* lbl = new Label(CLAY_IDI("label", i));
    //     lbl->SetText(lblTxt);
    //     g_window->Screen().Add(lbl);
    // }

    List myList;

    // Add columns
    ListColumn col1;
    col1.headerName = CLAY_STRING("Product");
    col1.width = 200.0f;
    myList.AddColumn(col1);

    ListColumn col2;
    col2.headerName = CLAY_STRING("Price");
    col2.width = 100.0f;
    myList.AddColumn(col2);

    // Add rows
    for (int i = 0; i < 20; i++)
    {
        myList.AddItem({CLAY_STRING("Laptop"), CLAY_STRING("$1200")});
        myList.AddItem({CLAY_STRING("Mouse"), CLAY_STRING("$25")});
        myList.AddItem({CLAY_STRING("Monitor"), CLAY_STRING("$300")});
    }

    // Configure styling
    myList.SetRowHeight(48);
    //myList.SetAlternateRowColor({45, 45, 45, 255});

    // Handle selection
    myList.SetOnSelectionChanged([](size_t index) {
        printf("Selected item: %zu\n", index);
    });

    g_window->Screen().Add(&myList);

    while (!g_window->ShouldClose()) {
        g_window->Render();
    }
    
    g_window->Close();

    delete g_theme;
    delete g_window;
    return 0;
}
