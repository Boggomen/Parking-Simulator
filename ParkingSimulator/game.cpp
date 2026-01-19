#include "game.hpp"
#include "globals.hpp"
#include "images.hpp"
#include <cmath>
#include <cstdio>

using namespace Gdiplus;

// Game state variables
Car playerCar;                // The player's car object
bool gameInitialized = false; // Whether the game is currently running
DWORD lastUpdateTime = 0;     // Time of last update (for calculating delta time)

// Reference to game background from images.cpp
extern HBITMAP hGBgImage;

// Sets up the game when player clicks Start
void InitGame(HWND hwnd) {
    if (gameInitialized) return;
    
    // Get window size and place car in the center
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float startX = (float)(clientRect.right / 2);
    float startY = (float)(clientRect.bottom / 2);
    
    // Initialize car facing upward (0 radians)
    playerCar.Init(startX, startY, 0.0f);
    
    // Try to load car image, fallback to rectangle if it fails
    if (!playerCar.LoadCarImage(L"resources/car.png")) {
        OutputDebugString(L"Warning: Could not load car.png, using fallback rendering\n");
    }
    
    // Start game timer (fires every 16ms for ~60 FPS)
    SetTimer(hwnd, GAME_TIMER_ID, GAME_TIMER_INTERVAL, NULL);
    lastUpdateTime = GetTickCount();
    
    gameInitialized = true;
}

// Cleans up when leaving the game (like pressing Escape)
void CleanupGame(HWND hwnd) {
    if (!gameInitialized) return;
    
    KillTimer(hwnd, GAME_TIMER_ID);
    gameInitialized = false;
}

// Checks which keys are currently pressed
// Uses GetAsyncKeyState for responsive continuous input
void ProcessGameInput(bool& accelerate, bool& brake, bool& steerLeft, bool& steerRight, bool& handbrake) {
    accelerate = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
    brake = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
    steerLeft = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000);
    steerRight = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000);
    handbrake = (GetAsyncKeyState(VK_SPACE) & 0x8000);
}

// Called every frame to update game logic
void UpdateGame(float deltaTime) {
    if (!gameInitialized) return;
    
    // Get input and update car physics
    bool accelerate, brake, steerLeft, steerRight, handbrake;
    ProcessGameInput(accelerate, brake, steerLeft, steerRight, handbrake);
    playerCar.Update(deltaTime, accelerate, brake, steerLeft, steerRight, handbrake);
}

// Draws everything on the game screen
void DrawGameScreen(HDC hdc, RECT* clientRect) {
    // Double buffering setup - draw to memory first to prevent flickering
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, clientRect->right, clientRect->bottom);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
    
    // Draw background (stretched to fill window)
    if (hGBgImage) {
        HDC hdcBg = CreateCompatibleDC(hdcMem);
        SelectObject(hdcBg, hGBgImage);
        
        BITMAP bm;
        GetObject(hGBgImage, sizeof(BITMAP), &bm);
        
        StretchBlt(hdcMem, 0, 0, clientRect->right, clientRect->bottom,
                   hdcBg, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        DeleteDC(hdcBg);
    } else {
        // Fallback: dark gray background
        HBRUSH brush = CreateSolidBrush(RGB(60, 60, 60));
        FillRect(hdcMem, clientRect, brush);
        DeleteObject(brush);
    }
    
    // Use GDI+ for smooth rotation and transparency
    Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    
    // Draw car
    playerCar.Draw(&graphics);
    
    // Draw speed display
    wchar_t speedText[64];
    swprintf_s(speedText, L"km/h: %.0f", (playerCar.speed / 10));
    
    Font font(L"Arial", 14);
    SolidBrush textBrush(Color(255, 255, 255, 255));
    SolidBrush bgBrush(Color(150, 0, 0, 0));
    
    graphics.FillRectangle(&bgBrush, 10.0f, 10.0f, 150.0f, 25.0f);
    graphics.DrawString(speedText, -1, &font, PointF(15.0f, 12.0f), &textBrush);
    
    // Draw control hints at bottom
    Font smallFont(L"Arial", 10);
    SolidBrush hintBrush(Color(200, 200, 200, 200));
    graphics.DrawString(L"WASD/Arrows: Drive | Space: Handbrake | Esc: Menu", -1, &smallFont, 
                       PointF(10.0f, (float)(clientRect->bottom - 25)), &hintBrush);
    
    // Copy buffer to screen and cleanup
    BitBlt(hdc, 0, 0, clientRect->right, clientRect->bottom, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
}

// Handles key presses during gameplay
void HandleGameKeyDown(HWND hwnd, WPARAM wParam) {
    if (wParam == VK_ESCAPE) {
        // Return to main menu
        CleanupGame(hwnd);
        currentScreen = SCREEN_MENU;
        InvalidateRect(hwnd, NULL, TRUE);
    }
}
