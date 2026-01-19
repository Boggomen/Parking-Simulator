#include "options.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "button.hpp"

// Options screen buttons
Button btnBack, btnLeft, btnRight;

void InitOptionsButtons() {
    InitButton(&btnBack, BTN_BACK, 60, 350, 135, 35, hBtnBack, L"Back");
    InitButton(&btnLeft, BTN_LEFT, 100, 200, 32, 32, hBtnLeft, L"<");
    InitButton(&btnRight, BTN_RIGHT, 250, 200, 32, 32, hBtnRight, L">");
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

    // Draw buttons
    DrawButton(hdc, &btnBack);
    DrawButton(hdc, &btnLeft);
    DrawButton(hdc, &btnRight);
}

BOOL HandleOptionsClick(HWND hwnd, int x, int y) {
    if (ButtonHitTest(&btnBack, x, y)) {
        return TRUE;  // Signal to go back to main menu
    }
    else if (ButtonHitTest(&btnLeft, x, y)) {
        // Handle left arrow (decrease setting)
    }
    else if (ButtonHitTest(&btnRight, x, y)) {
        // Handle right arrow (increase setting)
    }
    return FALSE;
}