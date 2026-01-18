#include <windows.h>
#include "windowHand.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "menu.hpp"

// The Program Start Here
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"ParkingGarageGameClass";

    // Create the window handler
    WindowHandler window;
                                              //Window Name       //Width, Height
    if (!window.Create(hInstance, CLASS_NAME, L"Parking Garage Game", 800, 600)) {
        return 0;
    }

    window.Show(nCmdShow);

    // Load images after showing window
    LoadImages();
    window.Invalidate();

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    FreeImages();
    return 0;
}