#include "options.hpp"
#include "globals.hpp"
#include "images.hpp"
#include "button.hpp"

// ============= Difficulty State =============
Difficulty currentDifficulty = DIFF_EASY;  // Actual game difficulty
Difficulty selectedDifficulty = DIFF_EASY; // Currently shown in options

// ============= Options Buttons =============
Button btnBack, btnLeft, btnRight, btnOk;

// ============= Initialize Function =============
void InitOptionsButtons() {
    // Back button - same position as Exit button on main menu
    InitButton(&btnBack, BTN_BACK, 60, 350, 135, 35, hBtnBack, L"Back");
    
    // Arrow buttons - left and right of difficulty display
    InitButton(&btnLeft, BTN_LEFT, 250, 200, 32, 32, hBtnLeft, L"<");
    InitButton(&btnRight, BTN_RIGHT, 450, 200, 32, 32, hBtnRight, L">");
    
    // OK button - under difficulty display
    InitButton(&btnOk, BTN_OK, 363, 280, 135, 35, hBtnOk, L"OK");
}

// ============= Get Difficulty Button Image =============
HBITMAP GetDifficultyButtonImage() {
    switch (selectedDifficulty) {
        case DIFF_EASY:     return hBtnEasy;
        case DIFF_MEDIUM:   return hBtnMedium;
        case DIFF_HARD:     return hBtnHard;
        case DIFF_CREATIVE: return hBtnCreative;
        default:            return hBtnEasy;
    }
}

// ============= Cycle Difficulty Function =============
void CycleDifficulty(int direction) {
    int diff = (int)selectedDifficulty;
    diff += direction;
    
    // Wrap around
    if (diff < 0) diff = 3;        // DIFF_CREATIVE
    if (diff > 3) diff = 0;        // DIFF_EASY
    
    selectedDifficulty = (Difficulty)diff;
}

// ============= Display Function =============
void DrawOptionsScreen(HDC hdc, RECT* clientRect) {
    // Draw background
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
        // Fallback color
        HBRUSH brush = CreateSolidBrush(RGB(40, 40, 60));
        FillRect(hdc, clientRect, brush);
        DeleteObject(brush);
    }

    // Draw difficulty button in center
    HBITMAP diffBtn = GetDifficultyButtonImage();
    if (diffBtn) {
        BITMAP bm;
        GetObject(diffBtn, sizeof(BITMAP), &bm);
        int diffX = (clientRect->right - bm.bmWidth) / 2;
        int diffY = 200;
        DrawBitmap(hdc, diffBtn, diffX, diffY, bm.bmWidth, bm.bmHeight);
    }
    else {
        // Fallback text for difficulty
        const wchar_t* diffText = L"Easy";
        switch (selectedDifficulty) {
            case DIFF_EASY:     diffText = L"Easy"; break;
            case DIFF_MEDIUM:   diffText = L"Medium"; break;
            case DIFF_HARD:     diffText = L"Hard"; break;
            case DIFF_CREATIVE: diffText = L"Creative"; break;
        }
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 350, 210, diffText, lstrlenW(diffText));
    }

    // Draw arrow buttons
    DrawButton(hdc, &btnLeft);
    DrawButton(hdc, &btnRight);
    
    // Draw OK button
    DrawButton(hdc, &btnOk);
    
    // Draw back button
    DrawButton(hdc, &btnBack);
}

// ============= Click Handler =============
BOOL HandleOptionsClick(HWND hwnd, int x, int y) {
    if (ButtonHitTest(&btnBack, x, y)) {
        return TRUE; // Go back to menu
    }
    else if (ButtonHitTest(&btnLeft, x, y)) {
        CycleDifficulty(-1);
        InvalidateRect(hwnd, NULL, TRUE);
    }
    else if (ButtonHitTest(&btnRight, x, y)) {
        CycleDifficulty(1);
        InvalidateRect(hwnd, NULL, TRUE);
    }
    else if (ButtonHitTest(&btnOk, x, y)) {
        // Confirm selection
        currentDifficulty = selectedDifficulty;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    return FALSE;
}