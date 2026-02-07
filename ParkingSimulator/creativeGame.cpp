#include "creativeGame.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "game.hpp"
#include <cmath>
#include <cstdio>

using namespace Gdiplus;

// ============= Creative Game State Variables =============
Car creativePlayerCar;
bool creativeGameInitialized = false;
DWORD creativeLastUpdateTime = 0;

// ============= Initialize Function =============
void InitCreativeGame(HWND hwnd) {
    if (creativeGameInitialized) return;

    // Get window size and place car in the center
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float startX = (float)(clientRect.right / 2);
    float startY = (float)(clientRect.bottom / 2);

    // Initialize car facing upward
    creativePlayerCar.Init(startX, startY, 0.0f);

    // Try to load car image
    if (!creativePlayerCar.LoadCarImage(L"resources/car.png")) {
        OutputDebugString(L"Warning: Could not load car.png, using fallback rendering\n");
    }

    // Start game timer
    SetTimer(hwnd, GAME_TIMER_ID, GAME_TIMER_INTERVAL, NULL);
    creativeLastUpdateTime = (DWORD)GetTickCount64();

    creativeGameInitialized = true;
}

// ============= Cleanup Function =============
void CleanupCreativeGame(HWND hwnd) {
    if (!creativeGameInitialized) return;

    KillTimer(hwnd, GAME_TIMER_ID);
    creativeGameInitialized = false;
}

// ============= Input Function =============
void ProcessCreativeGameInput(bool& accelerate, bool& brake, bool& steerLeft, bool& steerRight, bool& handbrake) {
    accelerate = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
    brake = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
    steerLeft = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000);
    steerRight = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000);
    handbrake = (GetAsyncKeyState(VK_SPACE) & 0x8000);
}

// ============= Update Function =============
void UpdateCreativeGame(float deltaTime) {
    if (!creativeGameInitialized) return;

    bool accelerate, brake, steerLeft, steerRight, handbrake;
    ProcessCreativeGameInput(accelerate, brake, steerLeft, steerRight, handbrake);
    creativePlayerCar.Update(deltaTime, accelerate, brake, steerLeft, steerRight, handbrake);
}

// ============= Display Function =============
void DrawCreativeGameScreen(HDC hdc, RECT* clientRect) {
    // Double buffering
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, clientRect->right, clientRect->bottom);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

    // Draw background based on difficulty
    HBITMAP bgImage = GetGameBackground();
    if (bgImage) {
        HDC hdcBg = CreateCompatibleDC(hdcMem);
        SelectObject(hdcBg, bgImage);

        BITMAP bm;
        GetObject(bgImage, sizeof(BITMAP), &bm);

        StretchBlt(hdcMem, 0, 0, clientRect->right, clientRect->bottom,
            hdcBg, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        DeleteDC(hdcBg);
    }
    else {
        // Fallback color
        HBRUSH brush = CreateSolidBrush(RGB(60, 60, 60));
        FillRect(hdcMem, clientRect, brush);
        DeleteObject(brush);
    }

    // GDI+ for rotation and transparency
    Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

    // Draw car
    creativePlayerCar.Draw(&graphics);

    // Draw speed display
    wchar_t speedText[64];
    swprintf_s(speedText, L"km/h: %.0f", (creativePlayerCar.speed / 10));

    Font font(L"Arial", 14);
    SolidBrush textBrush(Color(255, 255, 255, 255));
    SolidBrush bgBrush(Color(150, 0, 0, 0));

    graphics.FillRectangle(&bgBrush, 10.0f, 10.0f, 150.0f, 25.0f);
    graphics.DrawString(speedText, -1, &font, PointF(15.0f, 12.0f), &textBrush);

    // Draw difficulty indicator
    graphics.FillRectangle(&bgBrush, 10.0f, 40.0f, 100.0f, 25.0f);
    graphics.DrawString(L"Creative Mode", -1, &font, PointF(15.0f, 42.0f), &textBrush);

    // Draw controls at bottom
    Font smallFont(L"Arial", 10);
    SolidBrush hintBrush(Color(200, 200, 200, 200));
    graphics.DrawString(L"WASD/Arrows: Drive | Space: Handbrake | Esc: Menu", -1, &smallFont,
        PointF(10.0f, (float)(clientRect->bottom - 25)), &hintBrush);

    // Cleanup
    BitBlt(hdc, 0, 0, clientRect->right, clientRect->bottom, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
}

// ============= Key Handler =============
void HandleCreativeGameKeyDown(HWND hwnd, WPARAM wParam) {
    if (wParam == VK_ESCAPE) {
        CleanupCreativeGame(hwnd);
        currentScreen = SCREEN_MENU;
        InvalidateRect(hwnd, NULL, TRUE);
    }
}
