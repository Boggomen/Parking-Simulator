#include "menu.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "options.hpp"              

// Screen state - starts at main menu
ScreenState currentScreen = SCREEN_MENU;

// The actual button position variables live here
RECT btnStartRect = { 60, 250, 135, 35 };
RECT btnOptionsRect = { 60, 300, 135, 35 };
RECT btnExitRect = { 60, 350, 135, 35 };

BOOL PointInRect(int x, int y, RECT* r) {
    return (x >= r->left && x < r->left + r->right &&
        y >= r->top && y < r->top + r->bottom);
}

void DrawMainMenu(HDC hdc, RECT* clientRect) {
    // Draw background
    if (hBgImage) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBgImage);

        // Get the size of the background image
        BITMAP bm;
        GetObject(hBgImage, sizeof(BITMAP), &bm);

        // Stretch to fill the window
        StretchBlt(
            hdc,
            0, 0,
            clientRect->right, clientRect->bottom,
            hdcMem,
            0, 0,
            bm.bmWidth, bm.bmHeight,
            SRCCOPY
        );
        DeleteDC(hdcMem);
    }
    else {
        // if image didn't load fill with color
        HBRUSH brush = CreateSolidBrush(RGB(30, 30, 50));
        FillRect(hdc, clientRect, brush);
        DeleteObject(brush);
    }

    // Draw Start button
    if (hBtnStart) {
        DrawBitmap(hdc, hBtnStart,
            btnStartRect.left, btnStartRect.top,
            btnStartRect.right, btnStartRect.bottom);
    }
    else {
        // if image didn't load creates rectangle
        Rectangle(hdc,
            btnStartRect.left, btnStartRect.top,
            btnStartRect.left + btnStartRect.right,
            btnStartRect.top + btnStartRect.bottom);
        TextOut(hdc, btnStartRect.left + 30, btnStartRect.top + 15,
            L"Start Game", 10);
    }

    // Draw Options button
    if (hBtnOptions) {
        DrawBitmap(hdc, hBtnOptions,
            btnOptionsRect.left, btnOptionsRect.top,
            btnOptionsRect.right, btnOptionsRect.bottom);
    }
    else {
        // if image didn't load creates rectangle
        Rectangle(hdc,
            btnOptionsRect.left, btnOptionsRect.top,
            btnOptionsRect.left + btnOptionsRect.right,
            btnOptionsRect.top + btnOptionsRect.bottom);
        TextOut(hdc, btnOptionsRect.left + 40, btnOptionsRect.top + 15,
            L"Options", 7);
    }

    // Draw Exit button
    if (hBtnExit) {
        DrawBitmap(hdc, hBtnExit,
            btnExitRect.left, btnExitRect.top,
            btnExitRect.right, btnExitRect.bottom);
    }
    else {
        // if image didn't load creates rectangle
        Rectangle(hdc,
            btnExitRect.left, btnExitRect.top,
            btnExitRect.left + btnExitRect.right,
            btnExitRect.top + btnExitRect.bottom);
        TextOut(hdc, btnExitRect.left + 50, btnExitRect.top + 15,
            L"Exit", 4);
    }
}

void HandleMenuClick(HWND hwnd, int x, int y) {
    if (PointInRect(x, y, &btnStartRect)) {
        MessageBox(hwnd, L"Starting the game!", L"Start Game", MB_OK);
    }
    else if (PointInRect(x, y, &btnOptionsRect)) {
        currentScreen = SCREEN_OPTIONS;
        InvalidateRect(hwnd, NULL, TRUE);  // Redraw window
    }
    else if (PointInRect(x, y, &btnExitRect)) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }
}