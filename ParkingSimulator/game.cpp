#include "game.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "borders.hpp"
#include "creativeGame.hpp"
#include <cmath>
#include <cstdio>

using namespace Gdiplus;

// ============= Game State Variables =============
Car playerCar;
bool gameInitialized = false;
DWORD lastUpdateTime = 0;
Borders gameBorders;

// ============= Initialize Function =============
void InitGame(HWND hwnd) {
    if (currentDifficulty == DIFF_CREATIVE) {
        InitCreativeGame(hwnd);
        return;
    }
    if (gameInitialized) return;
    
    // Get window size
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    
    // Start car at bottom left, facing right (PI/2 radians)
    float startX = 100.0f;
    float startY = (float)clientRect.bottom - 60.0f;
    float startHeading = PI / 2.0f;  // Facing right
    
    // Initialize car
    playerCar.Init(startX, startY, startHeading);
    
    // Try to load car image
    if (!playerCar.LoadCarImage(L"resources/car.png")) {
        OutputDebugString(L"Warning: Could not load car.png, using fallback rendering\n");
    }
    
    // Initialize borders
    InitBorders(&gameBorders, &clientRect, playerCar.width, playerCar.height);
    
    // Start game timer
    SetTimer(hwnd, GAME_TIMER_ID, GAME_TIMER_INTERVAL, NULL);
    lastUpdateTime = GetTickCount64();
    
    gameInitialized = true;
}

// ============= Cleanup Function =============
void CleanupGame(HWND hwnd) {
    if (creativeGameInitialized) {
        CleanupCreativeGame(hwnd);
        return;
    }
    if (!gameInitialized) return;
    
    KillTimer(hwnd, GAME_TIMER_ID);
    gameInitialized = false;
}

// ============= Input Function =============
void ProcessGameInput(bool& accelerate, bool& brake, bool& steerLeft, bool& steerRight, bool& handbrake) {
    accelerate = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
    brake = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
    steerLeft = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000);
    steerRight = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000);
    handbrake = (GetAsyncKeyState(VK_SPACE) & 0x8000);
}

// ============= Update Function =============
void UpdateGame(float deltaTime) {
    if (creativeGameInitialized) {
        UpdateCreativeGame(deltaTime);
        return;
    }
    if (!gameInitialized) return;
    
    bool accelerate, brake, steerLeft, steerRight, handbrake;
    ProcessGameInput(accelerate, brake, steerLeft, steerRight, handbrake);
    playerCar.Update(deltaTime, accelerate, brake, steerLeft, steerRight, handbrake);
    
    // Apply border collision
    ApplyBorderCollision(&gameBorders, &playerCar.x, &playerCar.y, 
                         &playerCar.velocityX, &playerCar.velocityY);
}

// ============= Get Difficulty Text =============
const wchar_t* GetDifficultyText() {
    switch (currentDifficulty) {
        case DIFF_EASY:     return L"Easy Mode";
        case DIFF_MEDIUM:   return L"Medium Mode";
        case DIFF_HARD:     return L"Hard Mode";
        case DIFF_CREATIVE: return L"Creative Mode";
        default:            return L"Easy Mode";
    }
}

// ============= Display Function =============
void DrawGameScreen(HDC hdc, RECT* clientRect) {
    if (creativeGameInitialized) {
        DrawEasyCreativeScreen(hdc, clientRect);
        return;
    }
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
    playerCar.Draw(&graphics);
    
    // Draw speed display
    wchar_t speedText[64];
    swprintf_s(speedText, L"km/h: %.0f", (playerCar.speed / 10));
    
    Font font(L"Arial", 14);
    SolidBrush textBrush(Color(255, 255, 255, 255));
    SolidBrush bgBrush(Color(150, 0, 0, 0));
    
    graphics.FillRectangle(&bgBrush, 10.0f, 10.0f, 150.0f, 25.0f);
    graphics.DrawString(speedText, -1, &font, PointF(15.0f, 12.0f), &textBrush);
    
    // Draw difficulty indicator
    const wchar_t* diffText = GetDifficultyText();
    graphics.FillRectangle(&bgBrush, 10.0f, 40.0f, 130.0f, 25.0f);
    graphics.DrawString(diffText, -1, &font, PointF(15.0f, 42.0f), &textBrush);
    
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
void HandleGameKeyDown(HWND hwnd, WPARAM wParam) {
    if (creativeGameInitialized) {
        HandleCreativeGameKeyDown(hwnd, wParam);
        return;
    }
    if (wParam == VK_ESCAPE) {
        CleanupGame(hwnd);
        currentScreen = SCREEN_MENU;
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

