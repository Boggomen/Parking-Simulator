// Program entry point - sets up GDI+, window, and runs the message loop

#include <windows.h>
#include <gdiplus.h>
#include "windowHand.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "menu.hpp"
#include "options.hpp"

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// GDI+ token needed for shutdown
ULONG_PTR gdiplusToken;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR pCmdLine, int nCmdShow) {
    
    // Initialize GDI+ for PNG support and image rotation
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    const wchar_t CLASS_NAME[] = L"ParkingGarageGameClass";

    // Create the game window
    WindowHandler window;
    if (!window.Create(hInstance, CLASS_NAME, L"Parking Garage Game", 800, 600)) {
        GdiplusShutdown(gdiplusToken);
        return 0;
    }

    window.Show(nCmdShow);

    // Load images and set up buttons
    LoadImages();
    InitMenuButtons();
    InitOptionsButtons();
    window.Invalidate();

    // Message loop - keeps the program running
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