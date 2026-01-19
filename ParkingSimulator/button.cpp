#include "button.hpp"

void InitButton(Button* btn, int id, int x, int y, int width, int height,
                HBITMAP image, const wchar_t* text) {
    btn->id = id;
    btn->rect = { x, y, width, height };
    btn->image = image;
    btn->text = text;
}

void DrawButton(HDC hdc, Button* btn) {
    int x = btn->rect.left;
    int y = btn->rect.top;
    int w = btn->rect.right;
    int h = btn->rect.bottom;

    if (btn->image) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, btn->image);
        BitBlt(hdc, x, y, w, h, hdcMem, 0, 0, SRCCOPY);
        DeleteDC(hdcMem);
    }
    else {
        //draw rectangle with text, last resort
        Rectangle(hdc, x, y, x + w, y + h);
        if (btn->text) {
            int textLen = lstrlenW(btn->text);
            TextOut(hdc, x + 10, y + (h / 2) - 8, btn->text, textLen);
        }
    }
}

BOOL ButtonHitTest(Button* btn, int x, int y) {
    return (x >= btn->rect.left && x < btn->rect.left + btn->rect.right &&
            y >= btn->rect.top && y < btn->rect.top + btn->rect.bottom);
}