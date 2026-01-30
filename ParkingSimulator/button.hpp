#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <windows.h>

// ============= Button Structure =============
struct Button { 
    RECT rect;           // x, y, width, height
    HBITMAP image;       // Button image (can be NULL)
    const wchar_t* text; // Fallback text if no image
    int id;              // Button identifier
};

// ============= Declaration of Functions =============
void InitButton(Button* btn, int id, int x, int y, int width, int height, 
                HBITMAP image, const wchar_t* text);
void DrawButton(HDC hdc, Button* btn);
BOOL ButtonHitTest(Button* btn, int x, int y);

#endif