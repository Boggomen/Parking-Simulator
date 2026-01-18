#include "images.hpp"
#include "globals.hpp"

// The actual image handles - definitions for all extern declarations
HBITMAP hBgImage = NULL;
HBITMAP hOBgImage = NULL;      // Added - was missing
HBITMAP hBtnStart = NULL;
HBITMAP hBtnOptions = NULL;
HBITMAP hBtnExit = NULL;
HBITMAP hBtnBack = NULL;       // Added - was missing
HBITMAP hBtnRight = NULL;      // Added - was missing
HBITMAP hBtnLeft = NULL;       // Added - was missing

void LoadImages() {
    hBgImage = (HBITMAP)LoadImage(
        NULL,
        L"resources/mainmenu_bg.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hOBgImage = (HBITMAP)LoadImage(
        NULL,
        L"resources/options_bg.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnStart = (HBITMAP)LoadImage(
        NULL,
        L"resources/btn_start.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnOptions = (HBITMAP)LoadImage(
        NULL,
        L"resources/btn_options.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnExit = (HBITMAP)LoadImage(
        NULL,
        L"resources/btn_exit.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnBack = (HBITMAP)LoadImage(
        NULL,
        L"resources/btn_back.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnRight = (HBITMAP)LoadImage(
        NULL,
        L"resources/right_arrow.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    hBtnLeft = (HBITMAP)LoadImage(
        NULL,
        L"resources/left_arrow.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );
}

void FreeImages() {
    if (hBgImage) DeleteObject(hBgImage);
    if (hOBgImage) DeleteObject(hOBgImage);   // Fixed: was hBgImage (copy-paste bug)
    if (hBtnStart) DeleteObject(hBtnStart);
    if (hBtnOptions) DeleteObject(hBtnOptions);
    if (hBtnExit) DeleteObject(hBtnExit);
    if (hBtnBack) DeleteObject(hBtnBack);
    if (hBtnRight) DeleteObject(hBtnRight);
    if (hBtnLeft) DeleteObject(hBtnLeft);
}

void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int width, int height) {
    if (hBitmap == NULL) return;

    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);
    BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
}