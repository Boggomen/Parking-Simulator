#include <windows.h>
#include <gdiplus.h>
#include "windowHand.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "menu.hpp"
#include "options.hpp"

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// ============= GDI+ Token =============
ULONG_PTR gdiplusToken;

// ============= Main Function =============
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR pCmdLine, int nCmdShow) {
    
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    const wchar_t CLASS_NAME[] = L"ParkingGarageGameClass";

    // Create window
    WindowHandler window;
    if (!window.Create(hInstance, CLASS_NAME, L"Parking Garage Game", 800, 600)) {
        GdiplusShutdown(gdiplusToken);
        return 0;
    }

    window.Show(nCmdShow);

    // Load images and initialize buttons
    LoadImages();
    InitMenuButtons();
    InitOptionsButtons();
    window.Invalidate();

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    FreeImages();
    GdiplusShutdown(gdiplusToken);
    
    return 0;
}