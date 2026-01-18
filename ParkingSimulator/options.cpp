#include "options.hpp"
#include "globals.hpp"
#include "images.hpp"

// Button positions for options screen
RECT btnBackRect = { 60, 400, 135, 35 };
RECT btnLeftRect = { 100, 200, 16, 16 };
RECT btnRightRect = { 250, 200, 16, 16 };

void FreeOptionsImages() {
    if (hOBgImage) { DeleteObject(hOBgImage); hOBgImage = NULL; }
    if (hBtnBack) { DeleteObject(hBtnBack); hBtnBack = NULL; }
    if (hBtnLeft) { DeleteObject(hBtnLeft); hBtnLeft = NULL; }
    if (hBtnRight) { DeleteObject(hBtnRight); hBtnRight = NULL; }
}

void DrawOptionsScreen(HDC hdc, RECT* clientRect) {
    // Draw options background
    if (hOBgImage) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hOBgImage);

        BITMAP bm;
        GetObject(hOBgImage, sizeof(BITMAP), &bm);

        StretchBlt(hdc, 0, 0, clientRect->right, clientRect->bottom,
                   hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        DeleteDC(hdcMem);
    }
    else {
        HBRUSH brush = CreateSolidBrush(RGB(40, 40, 60));
        FillRect(hdc, clientRect, brush);
        DeleteObject(brush);
    }

    // Draw Back button
    if (hBtnBack) {
        DrawBitmap(hdc, hBtnBack, btnBackRect.left, btnBackRect.top,
                   btnBackRect.right, btnBackRect.bottom);
    }
    else {
        Rectangle(hdc, btnBackRect.left, btnBackRect.top,
                  btnBackRect.left + btnBackRect.right,
                  btnBackRect.top + btnBackRect.bottom);
        TextOut(hdc, btnBackRect.left + 45, btnBackRect.top + 10, L"Back", 4);
    }

    // Draw Left/Right arrow buttons for settings
    // Add your option controls here (volume, etc.)
}

BOOL HandleOptionsClick(HWND hwnd, int x, int y) {
    // Reuse PointInRect from menu.cpp (declared in menu.hpp)
    extern BOOL PointInRect(int x, int y, RECT* r);

    if (PointInRect(x, y, &btnBackRect)) {
        return TRUE;  // Signal to go back to main menu
    }
    else if (PointInRect(x, y, &btnLeftRect)) {
        // Handle left arrow (decrease setting)
    }
    else if (PointInRect(x, y, &btnRightRect)) {
        // Handle right arrow (increase setting)
    }
    return FALSE;
}