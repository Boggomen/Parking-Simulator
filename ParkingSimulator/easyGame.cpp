#include "easyGame.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "game.hpp"
#include <cmath>
#include <cstdio>

using namespace Gdiplus;

// ============= Easy Game State Variables =============
Car easyPlayerCar;
bool easyGameInitialized = false;
DWORD easyLastUpdateTime = 0;

// ============= Initialize Function =============
void InitEasyGame(HWND hwnd) {
    if (easyGameInitialized) return;
    
    // Get window size and place car in the center
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float startX = (float)(clientRect.right / 2);
    float startY = (float)(clientRect.bottom / 2);
    
    // Initialize car facing upward
    easyPlayerCar.Init(startX, startY, 0.0f);
    
    // Try to load car image
    if (!easyPlayerCar.LoadCarImage(L"resources/car.png")) {
        OutputDebugString(L"Warning: Could not load car.png, using fallback rendering\n");
    }
    
    // Start game timer
    SetTimer(hwnd, GAME_TIMER_ID, GAME_TIMER_INTERVAL, NULL);
    easyLastUpdateTime = GetTickCount64();
    
    easyGameInitialized = true;
}

// ============= Cleanup Function =============
void CleanupEasyGame(HWND hwnd) {
    if (!easyGameInitialized) return;
    
    KillTimer(hwnd, GAME_TIMER_ID);
    easyGameInitialized = false;
}

// ============= Input Function =============
void ProcessEasyGameInput(bool& accelerate, bool& brake, bool& steerLeft, bool& steerRight, bool& handbrake) {
    accelerate = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
    brake = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
    steerLeft = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000);
    steerRight = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000);
    handbrake = (GetAsyncKeyState(VK_SPACE) & 0x8000);
}

// ============= Update Function =============
void UpdateEasyGame(float deltaTime) {
    if (!easyGameInitialized) return;
    
    bool accelerate, brake, steerLeft, steerRight, handbrake;
    ProcessEasyGameInput(accelerate, brake, steerLeft, steerRight, handbrake);
    easyPlayerCar.Update(deltaTime, accelerate, brake, steerLeft, steerRight, handbrake);
}

// ============= Display Function =============
void DrawEasyGameScreen(HDC hdc, RECT* clientRect) {
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
    } else {
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
    easyPlayerCar.Draw(&graphics);
    
    // Draw speed display
    wchar_t speedText[64];
    swprintf_s(speedText, L"km/h: %.0f", (easyPlayerCar.speed / 10));
    
    Font font(L"Arial", 14);
    SolidBrush textBrush(Color(255, 255, 255, 255));
    
    graphics.DrawString(speedText, -1, &font, PointF(15.0f, 12.0f), &textBrush);
    
    // Draw difficulty indicator
    graphics.DrawString(L"Easy Mode", -1, &font, PointF(15.0f, 42.0f), &textBrush);
    
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
void HandleEasyGameKeyDown(HWND hwnd, WPARAM wParam) {
    if (wParam == VK_ESCAPE) {
        CleanupEasyGame(hwnd);
        currentScreen = SCREEN_MENU;
        InvalidateRect(hwnd, NULL, TRUE);
    }
}
