#include "menu.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "options.hpp"
#include "button.hpp"
#include "game.hpp"

// ============= Screen State =============
ScreenState currentScreen = SCREEN_MENU;

// ============= Menu Buttons =============
Button btnStart, btnOptions, btnExit;

// ============= Help Functions =============
// Checks if point is inside rectangle
BOOL PointInRect(int x, int y, RECT* r) {
    return (x >= r->left && x < r->left + r->right &&
        y >= r->top && y < r->top + r->bottom);
}

// ============= Display Functions =============
// Draws the main menu screen
void DrawMainMenu(HDC hdc, RECT* clientRect) {
    // Draw background
    if (hBgImage) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBgImage);

        BITMAP bm;
        GetObject(hBgImage, sizeof(BITMAP), &bm);

        StretchBlt(hdc, 0, 0, clientRect->right, clientRect->bottom,
                   hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        DeleteDC(hdcMem);
    }
    else {
        // Fallback color if image didn't load
        HBRUSH brush = CreateSolidBrush(RGB(30, 30, 50));
        FillRect(hdc, clientRect, brush);
        DeleteObject(brush);
    }

    // Draw buttons
    DrawButton(hdc, &btnStart);
    DrawButton(hdc, &btnOptions);
    DrawButton(hdc, &btnExit);
}

// ============= Click Handler =============
// Handles button clicks on main menu
void HandleMenuClick(HWND hwnd, int x, int y) {
    if (ButtonHitTest(&btnStart, x, y)) {
        currentScreen = SCREEN_GAME;
        InitGame(hwnd);
        InvalidateRect(hwnd, NULL, TRUE);
    }
    else if (ButtonHitTest(&btnOptions, x, y)) {
        currentScreen = SCREEN_OPTIONS;
        InvalidateRect(hwnd, NULL, TRUE);  
    }
    else if (ButtonHitTest(&btnExit, x, y)) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }
}

// ============= Initialize Function =============
// Initializes menu buttons with positions and images
void InitMenuButtons() {
    InitButton(&btnStart, BTN_START,     60, 250, 135, 35, hBtnStart, L"Start Game");
    InitButton(&btnOptions, BTN_OPTIONS, 60, 300, 135, 35, hBtnOptions, L"Options");
    InitButton(&btnExit, BTN_EXIT,       60, 350, 135, 35, hBtnExit, L"Exit");
}